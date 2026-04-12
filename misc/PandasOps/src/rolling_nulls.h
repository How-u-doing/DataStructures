#pragma once

#include <deque>
#include <functional>
#include <limits>
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

    void expire(uint32_t t) {
        // right closed: (t - window_ms, t]
        while (!buf_.empty() && buf_.front().first <= t - window_ms_) {
            if (!is_null(buf_.front().second))
                SumTraits::sub(sum_, buf_.front().second, compensation_sub_);
            else
                null_cnt_--;
            buf_.pop_front();
        }
    }

    void update(std::pair<uint32_t, T> x) {
        expire(x.first);

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

    void expire(uint32_t t) { sum_.expire(t); }

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

    void expire(uint32_t t) {
        // right closed: (t - window_ms, t]
        while (!is_null_.empty() && is_null_.front().first <= t - window_ms_) {
            if (is_null_.front().second)
                null_cnt_--;
            is_null_.pop_front();
        }

        while (!dq_.empty() && dq_.front().first <= t - window_ms_)
            dq_.pop_front();
    }

    void update(std::pair<uint32_t, T> x) {
        expire(x.first);

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

constexpr double InvCondTol = std::numeric_limits<double>::epsilon() * 1e3;

template <int N>
class RollingMoment {
    static_assert(N >= 2 && N <= 4, "RollingMoment only supports 2nd, 3rd, 4th moments");

public:
    RollingMoment() = default;

    // See https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
    void push(double x) {
        if (is_null(x))
            return;

        double old_moment = (N == 4) ? M4_ : (N == 3 ? M3_ : M2_);

        n_++;
        double delta = x - mean_;
        double delta_n = delta / n_;
        double delta_n2 = delta_n * delta_n;
        double term1 = delta * delta_n * (n_ - 1);

        mean_ += delta_n;
        if constexpr (N >= 4) {
            M4_ +=
                term1 * delta_n2 * (n_ * n_ - 3 * n_ + 3) + 6 * delta_n2 * M2_ - 4 * delta_n * M3_;
        }
        if constexpr (N >= 3) {
            M3_ += term1 * delta_n * (n_ - 2) - 3 * delta_n * M2_;
        }
        M2_ += term1;

        double new_moment = (N == 4) ? M4_ : (N == 3 ? M3_ : M2_);
        if (abs(old_moment) * InvCondTol > abs(new_moment))
            // possible catastrophic cancellation
            numerically_unstable_ = true;
    }

    void pop(double x) {
        if (is_null(x))
            return;

        if (n_ <= 1) {
            reset();
            return;
        }

        double old_moment = (N == 4) ? M4_ : (N == 3 ? M3_ : M2_);

        n_--;
        double delta = x - mean_;
        double delta_n = delta / n_;
        double term1 = delta_n * delta * (n_ + 1);

        mean_ -= delta_n;
        if constexpr (N >= 4) {
            M4_ -= delta_n * (delta_n * (term1 * (n_ * n_ + 3 * n_ + 3) - 6 * M2_) - 4 * M3_);
        }
        if constexpr (N >= 3) {
            M3_ -= delta_n * (term1 * (n_ + 2) - 3 * M2_);
        }
        M2_ -= term1;

        double new_moment = (N == 4) ? M4_ : (N == 3 ? M3_ : M2_);
        if ((abs(old_moment) + abs(new_moment - old_moment)) * InvCondTol > abs(new_moment))
            // possible catastrophic cancellation
            numerically_unstable_ = true;
    }

    void reset() {
        n_ = 0;
        mean_ = M2_ = M3_ = M4_ = 0.0;
        numerically_unstable_ = false;
    }

    void clear_numerical_instability() { numerically_unstable_ = false; }

    bool numerically_unstable() const { return numerically_unstable_; }

    template <typename Iter>
    void recalculate(Iter begin, Iter end) {
        reset();
        for (auto it = begin; it != end; ++it)
            push(*it);
        clear_numerical_instability();
    }

    int64_t count() const { return n_; }

    double sum() const { return mean_ * n_; }

    double mean() const { return mean_; }

    double var(bool biased = false) const {
        if (n_ < 2)
            return NAN;
        return M2_ / (biased ? n_ : (n_ - 1));
    }

    double std(bool biased = false) const {
        double v = var(biased);
        return std::isnan(v) ? v : std::sqrt(v);
    }

    double skew(bool biased = false) const {
        static_assert(N >= 3, "skewness requires N >= 3");

        if (n_ < 3)
            return NAN;

        double moments_ratio = M3_ / std::pow(M2_, 1.5);

        if (biased)
            return std::sqrt(n_) * moments_ratio;

        double correction = n_ * std::sqrt(n_ - 1.0) / (n_ - 2.0);
        return correction * moments_ratio;
    }

    double kurt(bool biased = false) const {
        static_assert(N >= 4, "kurtosis requires N >= 4");

        if (n_ < 4)
            return NAN;

        double g2 = n_ * M4_ / (M2_ * M2_);

        if (biased)
            return g2;

        double correction = (n_ - 1.0) / ((n_ - 2.0) * (n_ - 3.0));
        double term = (n_ + 1.0) * g2 - 3.0 * (n_ - 1.0);
        return correction * term;
    }

private:
    int64_t n_ = 0;
    double mean_ = 0.0;
    double M2_ = 0.0;
    double M3_ = 0.0;
    double M4_ = 0.0;
    bool numerically_unstable_ = false;
};

template <int N>
class TsRollingMoment {
public:
    explicit TsRollingMoment(uint32_t window_ms, uint32_t min_obs)
        : window_ms_(window_ms), min_obs_(min_obs) {}

    void expire(uint32_t t) {
        // right closed: (t - window_ms, t]
        while (!buf_.empty() && buf_.front().first <= t - window_ms_) {
            rm_.pop(buf_.front().second);
            buf_.pop_front();
        }
    }

    void update(std::pair<uint32_t, double> x) {
        expire(x.first);

        rm_.push(x.second);
        buf_.push_back(x);

        if (numerically_unstable())
            recalculate();
    }

    bool numerically_unstable() const { return rm_.numerically_unstable(); }

    // update();
    // ...
    // expire();
    // if (numerically_unstable())
    //     recalculate();
    // var() / skew() / kurt();
    void recalculate() {
        rm_.reset();
        for (auto [_, value] : buf_)
            rm_.push(value);
        rm_.clear_numerical_instability();
    }

    double var() const {
        if (rm_.count() < min_obs_)
            return NAN;
        return rm_.var();
    }

    double std() const {
        if (rm_.count() < min_obs_)
            return NAN;
        return rm_.std();
    }

    double skew() const {
        if (rm_.count() < min_obs_)
            return NAN;
        return rm_.skew();
    }

    double kurt() const {
        if (rm_.count() < min_obs_)
            return NAN;
        return rm_.kurt();
    }

private:
    uint32_t window_ms_;
    uint32_t min_obs_;
    RollingMoment<N> rm_;
    std::deque<std::pair<uint32_t, double>> buf_;  // (time, value)
};

}  // namespace rolling_nulls
