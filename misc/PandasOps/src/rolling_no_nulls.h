#pragma once

#include "statistics.h"
#include <deque>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

#include <cmath>
#include <cstdint>

namespace rolling_no_nulls {

struct SumTraits {
    static void kahan_add(double &sum, double value, double &compensation) {
        double y = value - compensation;
        double new_sum = sum + y;
        compensation = (new_sum - sum) - y;
        sum = new_sum;
    }

    template <typename T, typename S>
    static void add(S &sum, T value, double &compensation) {
        if constexpr (std::is_floating_point_v<T>) {
            kahan_add(sum, value, compensation);
        } else {
            sum += value;
        }
    }

    template <typename T, typename S>
    static void sub(S &sum, T value, double &compensation) {
        if constexpr (std::is_floating_point_v<T>) {
            kahan_add(sum, -value, compensation);
        } else {
            sum -= value;
        }
    }
};

template <typename T, typename S>
class RollingSum {
public:
    explicit RollingSum(uint32_t window) : window_(window) {}

    // user can replace NaN with 0
    void update(T x) {
        if (buf_.size() == window_) {
            SumTraits::sub(sum_, buf_.front(), compensation_sub_);
            buf_.pop_front();
        }

        buf_.push_back(x);
        SumTraits::add(sum_, x, compensation_add_);
    }

    S get() const { return sum_; }

    uint32_t count() const { return buf_.size(); }

private:
    uint32_t window_;
    std::deque<T> buf_;
    S sum_ = 0;
    double compensation_add_ = 0.0;
    double compensation_sub_ = 0.0;
};

template <typename T, typename S>
class TsRollingSum {
public:
    explicit TsRollingSum(uint32_t window_ms) : window_ms_(window_ms) {}

    // user can replace NaN with 0
    void update(std::pair<uint32_t, T> x) {
        // right closed: (t - window_ms, t]
        while (!buf_.empty() && x.first - window_ms_ >= buf_.front().first) {
            SumTraits::sub(sum_, buf_.front().second, compensation_sub_);
            buf_.pop_front();
        }

        buf_.push_back(x);
        SumTraits::add(sum_, x.second, compensation_add_);
    }

    S get() const { return sum_; }

    uint32_t count() const { return buf_.size(); }

private:
    uint32_t window_ms_;
    std::deque<std::pair<uint32_t, T>> buf_;
    S sum_ = 0;
    double compensation_add_ = 0.0;
    double compensation_sub_ = 0.0;
};

template <typename T, typename S>
class RollingMean {
public:
    explicit RollingMean(uint32_t window) : sum_(window) {}

    // if you replace NaN with 0, real_avg = get_sum() / (count() - 1)
    // or you can use the rolling_nulls::RollingMean version instead
    void update(T x) { sum_.update(x); }

    double get() const {
        uint32_t cnt = count();
        return cnt == 0 ? NAN : get_sum() / static_cast<double>(cnt);
    }

    S get_sum() const { return sum_.get(); }

    uint32_t count() const { return sum_.count(); }

private:
    RollingSum<T, S> sum_;
};

template <typename T, typename S>
class TsRollingMean {
public:
    explicit TsRollingMean(uint32_t window_ms) : sum_(window_ms) {}

    // if you replace NaN with 0, real_avg = get_sum() / (count() - 1)
    // or you can use the rolling_nulls::TsRollingMean version instead
    void update(std::pair<uint32_t, T> x) { sum_.update(x); }

    double get() const {
        uint32_t cnt = count();
        return cnt == 0 ? NAN : get_sum() / static_cast<double>(cnt);
    }

    S get_sum() const { return sum_.get(); }

    uint32_t count() const { return sum_.count(); }

private:
    TsRollingSum<T, S> sum_;
};

template <typename T, typename Compare>
class RollingMinMax {
public:
    explicit RollingMinMax(uint32_t window, Compare cmp = Compare{}) : window_(window), cmp_(cmp) {}

    // user can replace NaN with DBL_MAX/DBL_MIN
    void update(T x) {
        // remove the extremum if it's outside the current window
        if (!dq_.empty() && dq_.front().first + window_ <= curr_index_)
            dq_.pop_front();

        // add the current index to the deque while maintaining the monotonicity
        // e.g., add 2 into dq=[4, 2, 1]: pop 1, 2, push back the new 2
        while (!dq_.empty() && !cmp_(dq_.back().second, x))
            dq_.pop_back();

        dq_.push_back({curr_index_, x});
        curr_index_++;
    }

    T get() const { return dq_.front().second; }

private:
    uint32_t window_;
    Compare cmp_;
    std::deque<std::pair<uint32_t, T>> dq_;  // (index, value)
    uint32_t curr_index_ = 0;
};

template <typename T>
using RollingMin = RollingMinMax<T, std::less<T>>;

template <typename T>
using RollingMax = RollingMinMax<T, std::greater<T>>;

template <typename T, typename Compare>
class TsRollingMinMax {
public:
    explicit TsRollingMinMax(uint32_t window_ms, Compare cmp = Compare{})
        : window_ms_(window_ms), cmp_(cmp) {}

    // user can replace NaN with DBL_MAX/DBL_MIN
    void update(std::pair<uint32_t, T> x) {
        // right closed: (t - window_ms, t]
        while (!dq_.empty() && dq_.front().first <= x.first - window_ms_)
            dq_.pop_front();

        while (!dq_.empty() && !cmp_(dq_.back().second, x.second))
            dq_.pop_back();

        dq_.push_back(x);
    }

    T get() const { return dq_.front().second; }

private:
    uint32_t window_ms_;
    Compare cmp_;
    std::deque<std::pair<uint32_t, T>> dq_;  // (time, value)
};

template <typename T>
using TsRollingMin = TsRollingMinMax<T, std::less<T>>;

template <typename T>
using TsRollingMax = TsRollingMinMax<T, std::greater<T>>;

using stats::QuantileMethod;

/*
 *           16                                    20
 *        /      \                              /      \
 *      14        10                          16        10
 *     /   \     /  \      replace 8         /   \     /  \
 *    8     7   9    3    ---------->      14     7   9    3
 *   / \   /                with 20        / \   /
 *  2  4  1                               2  4  1
 *
 * Replace operation can be done by propagating the 'hole' to the apex of the relevant heap
 * and then compare and exchange the tops of the two heaps if necessary.
 *
 * max_heap_propagate_up() / min_heap_propagate_up() implements this logic.
 *
 * The two heaps are stored in a single array as follows:
 *
 *           |L|                 N - |L|
 *          max-heap            min-heap
 *      H[0], ..., H[L-1]    H[L], ..., H[N-1]
 */
template <typename T>
class RollingQuantile {
public:
    explicit RollingQuantile(uint32_t window, double q, QuantileMethod method)
        : window_(window), method_(method) {
        q = std::clamp(q, 0.0, 1.0);
        float_idx_ = (window - 1) * q;

        if (method == QuantileMethod::Nearest)
            q_idx_ = static_cast<uint32_t>(std::round(float_idx_));
        else if (method == QuantileMethod::Higher)
            q_idx_ = static_cast<uint32_t>(std::ceil(float_idx_));
        else  // Lower | Midpoint | Linear
            q_idx_ = static_cast<uint32_t>(float_idx_);

        ringbuf_.resize(window_);
        pos_.resize(window_, -1);
        h_.resize(window_, -1);
    }

    void update(T x) {
        if (count_ < window_) {
            ringbuf_[curr_index_] = x;
            pos_[curr_index_] = curr_index_;
            h_[curr_index_] = curr_index_;
            if (count_ < q_idx_ + 1)
                // insert x into max heap
                max_heap_sift_up(curr_index_);
            else
                // insert x into min heap
                min_heap_propagate_up(curr_index_);
            count_++;
        } else {
            // replace ringbuf_[curr_index_] with x
            T old = ringbuf_[curr_index_];
            ringbuf_[curr_index_] = x;
            uint32_t pos = pos_[curr_index_];

            if (pos < q_idx_ + 1) {  // in max heap
                if (x > old)
                    max_heap_propagate_up(pos);
                else
                    max_heap_sift_down(pos);
            } else {  // in min heap
                if (x < old)
                    min_heap_propagate_up(pos);
                else
                    min_heap_sift_down(pos);
            }
        }
        advance_index();
    }

    double get() const {
        if (count_ < window_)
            return NAN;

        double max_heap_top = get_max_heap_top();
        if (q_idx_ == static_cast<uint32_t>(std::ceil(float_idx_)))
            return max_heap_top;

        // q_idx_ < float_idx_ < q_idx_ + 1 <= window_ - 1
        if (method_ == QuantileMethod::Midpoint || method_ == QuantileMethod::Linear) {
            double next = get_min_heap_top();
            double g = (method_ == QuantileMethod::Linear) ? (float_idx_ - q_idx_) : 0.5;
            return stats::linear_interpolation(max_heap_top, next, g);
        }

        return max_heap_top;
    }

private:
    void max_heap_propagate_up(uint32_t pos) {
        bool hit_top = max_heap_sift_up(pos);
        if (hit_top && get_max_heap_top() > get_min_heap_top()) {
            heap_xchg(0, q_idx_ + 1);
            min_heap_sift_down(q_idx_ + 1);
        }
    }

    void min_heap_propagate_up(uint32_t pos) {
        bool hit_top = min_heap_sift_up(pos);
        if (hit_top && get_min_heap_top() < get_max_heap_top()) {
            heap_xchg(0, q_idx_ + 1);
            max_heap_sift_down(0);
        }
    }

    bool max_heap_sift_up(uint32_t i) { return heap_sift_up(i, 0, std::greater<T>{}); }

    bool min_heap_sift_up(uint32_t i) { return heap_sift_up(i, q_idx_ + 1, std::less<T>{}); }

    void max_heap_sift_down(uint32_t i) { heap_sift_down(i, 0, q_idx_ + 1, std::greater<T>{}); }

    void min_heap_sift_down(uint32_t i) {
        heap_sift_down(i, q_idx_ + 1, window_ - q_idx_ - 1, std::less<T>{});
    }

    template <typename Compare>
    bool heap_sift_up(uint32_t i, uint32_t base, Compare cmp) {
        i -= base;
        while (i > 0) {
            uint32_t parent = (i - 1) / 2;
            if (!cmp(ringbuf_[h_[i + base]], ringbuf_[h_[parent + base]]))
                break;
            heap_xchg(i + base, parent + base);
            i = parent;
        }
        bool hit_top = (i == 0);
        return hit_top;
    }

    template <typename Compare>
    void heap_sift_down(uint32_t i, uint32_t base, uint32_t n, Compare cmp) {
        i -= base;
        uint32_t child;
        while ((child = 2 * i + 1) < n) {
            if (child + 1 < n && cmp(ringbuf_[h_[child + 1 + base]], ringbuf_[h_[child + base]]))
                child++;
            if (!cmp(ringbuf_[h_[child + base]], ringbuf_[h_[i + base]]))
                break;
            heap_xchg(i + base, child + base);
            i = child;
        }
    }

    void heap_xchg(uint32_t i, uint32_t j) {
        std::swap(h_[i], h_[j]);
        pos_[h_[i]] = i;
        pos_[h_[j]] = j;
    }

    void advance_index() {
        // window_ isn't a compile-time constant, modulo may be more expensive than
        // a conditional branch or cmov instruction
        // curr_index_ = (curr_index_ + 1) % window_;
        curr_index_++;
        if (curr_index_ == window_)
            curr_index_ = 0;
    }

    T get_max_heap_top() const { return ringbuf_[h_[0]]; }

    T get_min_heap_top() const { return ringbuf_[h_[q_idx_ + 1]]; }

    uint32_t window_;
    QuantileMethod method_;
    double float_idx_;
    uint32_t q_idx_;
    uint32_t curr_index_ = 0;
    uint32_t count_ = 0;
    std::vector<T> ringbuf_;
    // pos_[i] locates the position of ringbuf_[i] in the heap
    std::vector<uint32_t> pos_;
    // h_[pos_[i]] = i, so when we swap h_[a] and h_[b], positions of a and b
    // can be easily updated by exchanging pos_[h_[a]] <-> pos_[h_[b]]
    // h_[0..q_idx_]: max-heap, h_[q_idx_+1..window_-1]: min-heap
    std::vector<uint32_t> h_;
};

}  // namespace rolling_no_nulls
