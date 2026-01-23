#pragma once

#include <deque>
#include <functional>
#include <type_traits>
#include <utility>

#include <cmath>
#include <cstdint>

namespace rolling_nulls {

template <typename T>
constexpr bool is_null(T value) {
    if constexpr (std::is_floating_point_v<T>) {
        return std::isnan(value);
    } else {
        return false;
    }
}

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
    explicit RollingSum(uint32_t window, uint32_t min_obs = 0)
        : window_(window), min_obs_(min_obs == 0 ? window_ : min_obs) {}

    void update(T x) {
        if (buf_.size() == window_) {
            if (!is_null(buf_.front()))
                SumTraits::sub(sum_, buf_.front(), compensation_sub_);
            else
                null_cnt_--;
            buf_.pop_front();
        }

        buf_.push_back(x);
        if (!is_null(x))
            SumTraits::add(sum_, x, compensation_add_);
        else
            null_cnt_++;
    }

    S get_sum() const { return sum_; }

    double get() const { return non_null_count() >= min_obs_ ? static_cast<double>(sum_) : NAN; }

    uint32_t non_null_count() const { return buf_.size() - null_cnt_; }

private:
    uint32_t window_;
    uint32_t min_obs_;
    std::deque<T> buf_;
    S sum_ = 0;
    double compensation_add_ = 0.0;
    double compensation_sub_ = 0.0;
    uint32_t null_cnt_ = 0;
};

template <typename T, typename S>
class TsRollingSum {
public:
    explicit TsRollingSum(uint32_t window_ms, uint32_t min_obs = 1)
        : window_ms_(window_ms), min_obs_(min_obs) {}

    void update(std::pair<uint32_t, T> x) {
        // right closed: (t - window_ms, t]
        while (!buf_.empty() && x.first - window_ms_ >= buf_.front().first) {
            if (!is_null(buf_.front().second))
                SumTraits::sub(sum_, buf_.front().second, compensation_sub_);
            else
                null_cnt_--;
            buf_.pop_front();
        }

        buf_.push_back(x);
        if (!is_null(x.second))
            SumTraits::add(sum_, x.second, compensation_add_);
        else
            null_cnt_++;
    }

    S get_sum() const { return sum_; }

    double get() const { return non_null_count() >= min_obs_ ? static_cast<double>(sum_) : NAN; }

    uint32_t non_null_count() const { return buf_.size() - null_cnt_; }

private:
    uint32_t window_ms_;
    uint32_t min_obs_;
    std::deque<std::pair<uint32_t, T>> buf_;
    S sum_ = 0;
    double compensation_add_ = 0.0;
    double compensation_sub_ = 0.0;
    uint32_t null_cnt_ = 0;
};

template <typename T, typename S>
class RollingMean {
public:
    explicit RollingMean(uint32_t window, uint32_t min_obs = 0) : sum_(window, min_obs) {}

    void update(T x) { sum_.update(x); }

    double get() const {
        uint32_t cnt = sum_.non_null_count();
        return cnt == 0 ? NAN : sum_.get() / static_cast<double>(cnt);
    }

private:
    RollingSum<T, S> sum_;
};

template <typename T, typename S>
class TsRollingMean {
public:
    explicit TsRollingMean(uint32_t window_ms, uint32_t min_obs = 1) : sum_(window_ms, min_obs) {}

    void update(std::pair<uint32_t, T> x) { sum_.update(x); }

    double get() const {
        uint32_t cnt = sum_.non_null_count();
        return cnt == 0 ? NAN : sum_.get() / static_cast<double>(cnt);
    }

private:
    TsRollingSum<T, S> sum_;
};

template <typename T, typename Compare>
class RollingMinMax {
public:
    explicit RollingMinMax(uint32_t window, uint32_t min_obs = 0, Compare cmp = Compare{})
        : window_(window), min_obs_(min_obs == 0 ? window_ : min_obs), cmp_(cmp) {}

    void update(T x) {
        if (is_null_.size() == window_) {
            if (is_null_.front())
                null_cnt_--;
            is_null_.pop_front();
        }

        // remove the extremum if it's outside the current window
        if (!dq_.empty() && dq_.front().first + window_ <= curr_index_)
            dq_.pop_front();

        if (!is_null(x)) {
            // add the current index to the deque while maintaining the monotonicity
            // e.g., add 2 into dq=[4, 2, 1]: pop 1, 2, push back the new 2
            while (!dq_.empty() && !cmp_(dq_.back().second, x))
                dq_.pop_back();

            dq_.push_back({curr_index_, x});
            is_null_.push_back(false);
        } else {
            is_null_.push_back(true);
            null_cnt_++;
        }

        curr_index_++;
    }

    double get() const {
        if (non_null_count() >= min_obs_ && !dq_.empty())
            return dq_.front().second;
        return NAN;
    }

    uint32_t non_null_count() const { return is_null_.size() - null_cnt_; }

private:
    uint32_t window_;
    uint32_t min_obs_;
    Compare cmp_;
    std::deque<std::pair<uint32_t, T>> dq_;  // (index, value)
    std::deque<bool> is_null_;
    uint32_t curr_index_ = 0;
    uint32_t null_cnt_ = 0;
};

template <typename T>
using RollingMin = RollingMinMax<T, std::less<T>>;

template <typename T>
using RollingMax = RollingMinMax<T, std::greater<T>>;

template <typename T, typename Compare>
class TsRollingMinMax {
public:
    explicit TsRollingMinMax(uint32_t window_ms, uint32_t min_obs = 1, Compare cmp = Compare{})
        : window_ms_(window_ms), min_obs_(min_obs), cmp_(cmp) {}

    void update(std::pair<uint32_t, T> x) {
        // right closed: (t - window_ms, t]
        while (!is_null_.empty() && is_null_.front().first <= x.first - window_ms_) {
            if (is_null_.front().second)
                null_cnt_--;
            is_null_.pop_front();
        }

        // remove the extremum if it's outside the current window
        while (!dq_.empty() && dq_.front().first <= x.first - window_ms_)
            dq_.pop_front();

        if (!is_null(x.second)) {
            while (!dq_.empty() && !cmp_(dq_.back().second, x.second))
                dq_.pop_back();

            dq_.push_back(x);
            is_null_.push_back({x.first, false});
        } else {
            is_null_.push_back({x.first, true});
            null_cnt_++;
        }
    }

    double get() const {
        if (non_null_count() >= min_obs_ && !dq_.empty())
            return dq_.front().second;
        return NAN;
    }

    uint32_t non_null_count() const { return is_null_.size() - null_cnt_; }

private:
    uint32_t window_ms_;
    uint32_t min_obs_;
    Compare cmp_;
    std::deque<std::pair<uint32_t, T>> dq_;  // (time, value)
    std::deque<std::pair<uint32_t, bool>> is_null_;
    uint32_t null_cnt_ = 0;
};

template <typename T>
using TsRollingMin = TsRollingMinMax<T, std::less<T>>;

template <typename T>
using TsRollingMax = TsRollingMinMax<T, std::greater<T>>;

}  // namespace rolling_nulls
