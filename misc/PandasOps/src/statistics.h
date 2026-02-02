#pragma once

#include <algorithm>
#include <functional>
#include <cmath>

namespace stats {

extern "C" void select_nth_unstable_rust(double *ptr, size_t len, size_t index);

// std::nth_element() signature
inline void select_nth_unstable_rs(double *first, double *nth, double *last) {
    select_nth_unstable_rust(first, last - first, nth - first);
}

template <typename T, typename Fn>
double median_impl(T *data, size_t n, Fn partition_fn) {
    size_t m = n / 2;
    partition_fn(data, data + m, data + n);
    // odd
    if (n % 2 == 1)
        return data[m];
    // even
    return (data[m] + *std::max_element(data, data + m)) / 2.0;
}

// Note that this routine modifies the data array.
// Pass a copy of the array if you don't want your original data get modified.
inline double median(double *data, size_t n) {
    // median_impl(data, n, std::nth_element<double>);
    return median_impl(data, n, select_nth_unstable_rs);
}

enum class QuantileMethod {
    Nearest,
    Lower,
    Higher,
    Midpoint,
    Linear,
};

// (1-t)*a + t*b
inline double linear_interpolation(double a, double b, double t) {
    if (a == b)
        return a;
    return a + t * (b - a);
}

// Note that this routine modifies the data array.
// Pass a copy of the array if you don't want your original data get modified.
// If your data contains NaNs, you can pass a copy of the data array with NaNs removed.
// Or you can partition NaNs to the end of the array and pass only the non-NaN part.
double quantile(double *data, size_t n, double q, QuantileMethod method = QuantileMethod::Linear);

double nanquantile(double *data, size_t n, double q,
                   QuantileMethod method = QuantileMethod::Linear);

}  // namespace stats
