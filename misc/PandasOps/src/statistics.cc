#include "statistics.h"
#include <stdexcept>

namespace stats {

// See https://numpy.org/devdocs/reference/generated/numpy.quantile.html
// (1-g)*y[j] + g*y[j+1]
// where the index j and coefficient g are the integral and fractional components of q*(n-1)
//   lower: g = 0
//   midpoint: g = 0.5
//   higher: g = 1
//   nearest: g = (q*(n-1) % 1) > 0.5
double quantile(double *data, size_t n, double q, QuantileMethod method) {
    if (!(q >= 0.0 && q <= 1.0))
        throw std::invalid_argument("quantile must be in [0, 1]");

    if (n == 0)
        return NAN;
    if (n == 1)
        return data[0];

    const double float_idx = (n - 1) * q;
    size_t q_idx;
    if (method == QuantileMethod::Nearest)
        q_idx = static_cast<size_t>(std::round(float_idx));
    else if (method == QuantileMethod::Higher)
        q_idx = static_cast<size_t>(std::ceil(float_idx));
    else  // Lower | Midpoint | Linear
        q_idx = static_cast<size_t>(float_idx);

    // partition
    // std::nth_element(data, data + q_idx, data + n);
    select_nth_unstable_rs(data, data + q_idx, data + n);

    // falls exactly on a data point, no interpolation is needed
    if (q_idx == static_cast<size_t>(std::ceil(float_idx)))
        return data[q_idx];

    // q_idx < float_idx < q_idx + 1 <= n - 1
    if (method == QuantileMethod::Midpoint || method == QuantileMethod::Linear) {
        double next = *std::min_element(data + q_idx + 1, data + n);
        double g = (method == QuantileMethod::Linear) ? (float_idx - q_idx) : 0.5;
        return linear_interpolation(data[q_idx], next, g);
    }

    // Nearest | Lower
    return data[q_idx];
}

double nanquantile(double *data, size_t n, double q, QuantileMethod method) {
    // partition NaNs to the end
    // std::partition uses Hoare's bidirectional partitioning scheme, which does
    // fewer (1/3 of) swaps than Lomuto's partition scheme on average.
    double *pos = std::partition(data, data + n, [](double v) { return !std::isnan(v); });

    size_t non_nan_count = pos - data;
    return quantile(data, non_nan_count, q, method);
}

}  // namespace stats