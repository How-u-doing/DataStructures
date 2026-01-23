#pragma once

#include <deque>
#include <functional>
#include <type_traits>
#include <utility>

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

}  // namespace rolling_no_nulls
