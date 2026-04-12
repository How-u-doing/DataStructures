#include "statistics.h"
#include "rolling_no_nulls.h"
#include "rolling_nulls.h"
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

using SumF64 = rolling_nulls::RollingSum<double, double>;
using MeanF64 = rolling_nulls::RollingMean<double, double>;
using MinF64 = rolling_nulls::RollingMin<double>;
using MaxF64 = rolling_nulls::RollingMax<double>;

using TsSumU32 = rolling_nulls::TsRollingSum<uint32_t, uint64_t>;
using TsSumF64 = rolling_nulls::TsRollingSum<double, double>;
using TsMeanU32 = rolling_nulls::TsRollingMean<uint32_t, uint64_t>;
using TsMeanF64 = rolling_nulls::TsRollingMean<double, double>;

using TsMinU32 = rolling_nulls::TsRollingMin<uint32_t>;
using TsMinF64 = rolling_nulls::TsRollingMin<double>;
using TsMaxU32 = rolling_nulls::TsRollingMax<uint32_t>;
using TsMaxF64 = rolling_nulls::TsRollingMax<double>;

template <typename X, typename R>
static py::array_t<double> rolling_apply(py::array_t<X> x, uint32_t window, uint32_t min_obs = 0) {
    py::buffer_info buf_info = x.request();
    const X *x_data = static_cast<const X *>(buf_info.ptr);

    py::array_t<double> res(buf_info.size);
    double *res_data = static_cast<double *>(res.request().ptr);

    R r(window, min_obs);
    for (py::ssize_t i = 0; i < buf_info.size; i++) {
        r.update(x_data[i]);
        res_data[i] = r.get();
    }

    return res;
}

template <typename X, typename R>
static py::array_t<double> ts_rolling_apply(py::array_t<uint32_t> t, py::array_t<X> x,
                                            uint32_t window_ms, uint32_t min_obs = 1) {
    py::buffer_info buf_info = t.request();
    const uint32_t *t_data = static_cast<const uint32_t *>(buf_info.ptr);
    const X *x_data = static_cast<const X *>(x.request().ptr);

    py::array_t<double> res(buf_info.size);
    double *res_data = static_cast<double *>(res.request().ptr);

    R r(window_ms, min_obs);
    for (py::ssize_t i = 0; i < buf_info.size; i++) {
        r.update({t_data[i], x_data[i]});
        res_data[i] = r.get();
    }

    return res;
}

static double median(py::array_t<double> x) {
    py::buffer_info buf_info = x.request();
    const double *x_data = static_cast<const double *>(buf_info.ptr);

    std::vector<double> x_copy(x_data, x_data + buf_info.size);

    return stats::median(x_copy.data(), x_copy.size());
}

static double quantile(py::array_t<double> x, double q, stats::QuantileMethod method) {
    py::buffer_info buf_info = x.request();
    const double *x_data = static_cast<const double *>(buf_info.ptr);

    std::vector<double> x_copy(x_data, x_data + buf_info.size);

    return stats::quantile(x_copy.data(), x_copy.size(), q, method);
}

static double nanquantile(py::array_t<double> x, double q, stats::QuantileMethod method) {
    py::buffer_info buf_info = x.request();
    double *x_data = static_cast<double *>(buf_info.ptr);

    return stats::nanquantile(x_data, buf_info.size, q, method);
}

static py::array_t<double> rolling_quantile(
    py::array_t<double> x, uint32_t window, double q,
    stats::QuantileMethod method = stats::QuantileMethod::Linear) {
    py::buffer_info buf_info = x.request();
    const double *x_data = static_cast<const double *>(x.request().ptr);

    py::array_t<double> res(buf_info.size);
    double *res_data = static_cast<double *>(res.request().ptr);

    rolling_no_nulls::RollingQuantile<double> rq(window, q, method);

    for (py::ssize_t i = 0; i < buf_info.size; i++) {
        rq.update(x_data[i]);
        res_data[i] = rq.get();
    }

    return res;
}

template <int N>
static py::array_t<double> rolling_moment(py::array_t<double> x, uint32_t window) {
    py::buffer_info buf_info = x.request();
    const double *x_data = static_cast<const double *>(buf_info.ptr);

    py::array_t<double> res(buf_info.size);
    double *res_data = static_cast<double *>(res.request().ptr);

    rolling_nulls::RollingMoment<N> rm;
    for (py::ssize_t i = 0; i < buf_info.size; i++) {
        rm.push(x_data[i]);
        if (i >= window) {
            rm.pop(x_data[i - window]);

            if (rm.numerically_unstable())
                rm.recalculate(x_data + i - window + 1, x_data + i + 1);
        }

        if constexpr (N == 2) {
            res_data[i] = rm.var();
        } else if constexpr (N == 3) {
            res_data[i] = rm.skew();
        } else if constexpr (N == 4) {
            res_data[i] = rm.kurt();
        }
    }

    return res;
}

template <int N>
static py::array_t<double> ts_rolling_moment(py::array_t<uint32_t> t, py::array_t<double> x,
                                             uint32_t window_ms, uint32_t min_obs = 1) {
    py::buffer_info buf_info = t.request();
    const uint32_t *t_data = static_cast<const uint32_t *>(buf_info.ptr);
    const double *x_data = static_cast<const double *>(x.request().ptr);

    py::array_t<double> res(buf_info.size);
    double *res_data = static_cast<double *>(res.request().ptr);

    rolling_nulls::TsRollingMoment<N> rm(window_ms, min_obs);
    for (py::ssize_t i = 0; i < buf_info.size; i++) {
        rm.update({t_data[i], x_data[i]});

        if (rm.numerically_unstable())
            rm.recalculate();

        if constexpr (N == 2) {
            res_data[i] = rm.var();
        } else if constexpr (N == 3) {
            res_data[i] = rm.skew();
        } else if constexpr (N == 4) {
            res_data[i] = rm.kurt();
        }
    }

    return res;
}

PYBIND11_MODULE(ops, m) {
    py::enum_<stats::QuantileMethod>(m, "QuantileMethod")
        .value("Nearest", stats::QuantileMethod::Nearest)
        .value("Lower", stats::QuantileMethod::Lower)
        .value("Higher", stats::QuantileMethod::Higher)
        .value("Midpoint", stats::QuantileMethod::Midpoint)
        .value("Linear", stats::QuantileMethod::Linear);

    m.def("median", &median, py::arg("x"));
    m.def("quantile", &quantile, py::arg("x"), py::arg("q"),
          py::arg("method") = stats::QuantileMethod::Linear);
    // nanquantile modifies the array in-place
    m.def("nanquantile", &nanquantile, py::arg("x"), py::arg("q"),
          py::arg("method") = stats::QuantileMethod::Linear);

    m.def("rolling_quantile", &rolling_quantile, py::arg("x"), py::arg("window"), py::arg("q"),
          py::arg("method") = stats::QuantileMethod::Linear);

    m.def("rolling_sum", &rolling_apply<double, SumF64>, py::arg("x"), py::arg("window"),
          py::arg("min_obs") = 0);

    m.def("rolling_mean", &rolling_apply<double, MeanF64>, py::arg("x"), py::arg("window"),
          py::arg("min_obs") = 0);

    m.def("rolling_min", &rolling_apply<double, MinF64>, py::arg("x"), py::arg("window"),
          py::arg("min_obs") = 0);

    m.def("rolling_max", &rolling_apply<double, MaxF64>, py::arg("x"), py::arg("window"),
          py::arg("min_obs") = 0);

    m.def("rolling_var", &rolling_moment<2>, py::arg("x"), py::arg("window"));
    m.def("rolling_skew", &rolling_moment<3>, py::arg("x"), py::arg("window"));
    m.def("rolling_kurt", &rolling_moment<4>, py::arg("x"), py::arg("window"));

    // time series
    m.def("ts_rolling_sum", &ts_rolling_apply<uint32_t, TsSumU32>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_sum", &ts_rolling_apply<double, TsSumF64>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_mean", &ts_rolling_apply<uint32_t, TsMeanU32>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_mean", &ts_rolling_apply<double, TsMeanF64>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_min", &ts_rolling_apply<uint32_t, TsMinU32>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_min", &ts_rolling_apply<double, TsMinF64>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_max", &ts_rolling_apply<uint32_t, TsMaxU32>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_max", &ts_rolling_apply<double, TsMaxF64>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);

    m.def("ts_rolling_var", &ts_rolling_moment<2>, py::arg("t"), py::arg("x"), py::arg("window_ms"),
          py::arg("min_obs") = 1);
    m.def("ts_rolling_skew", &ts_rolling_moment<3>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);
    m.def("ts_rolling_kurt", &ts_rolling_moment<4>, py::arg("t"), py::arg("x"),
          py::arg("window_ms"), py::arg("min_obs") = 1);
}
