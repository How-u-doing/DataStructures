#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <vector>
#include <algorithm>

namespace py = pybind11;

// make high = median of {low, mid, high}
template <typename RandomIt, typename Compare>
void mid3(RandomIt low, RandomIt high, Compare comp) {
    auto mid = low + ((high - low) >> 1);
    if (comp(*mid, *low))
        std::iter_swap(low, mid);
    if (comp(*high, *low))
        std::iter_swap(low, high);
    if (comp(*mid, *high))
        std::iter_swap(mid, high);
}

template <typename RandomIt, typename Compare>
RandomIt Lomuto_partition(RandomIt low, RandomIt high, Compare comp) {
    // See 'CLRS-3e' p171-172 (illustration, Fig7.1).
    // See also <https://en.wikipedia.org/wiki/quicksort#Lomuto_partition_scheme>.
    mid3(low, high, comp);  // make high = median of {low, mid, high}
    auto pivot = *high;     // choose the rightmost element as pivot
    auto i = low;
    for (auto j = low; j != high; ++j) {
        if (comp(*j, pivot))
            std::iter_swap(i++, j);
    }
    std::iter_swap(i, high);
    return i;
    // Now a[lo..i-1] <= pivot, *i==pivot, a[i+1..hi] >= pivot. (operator<, ascending)
    // Then quicksort a[lo..i-1] & a[i+1..hi].
}

// same effect as std::nth_element
// see https://en.wikipedia.org/wiki/Quickselect
// O(n) on average, O(n^2) worst-case
template <typename RandomIt,
          typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quickselect(RandomIt first, RandomIt kth, RandomIt last, Compare comp = Compare{}) {
    while (true) {
        if (last - first < 2)
            return;
        auto pivot = Lomuto_partition(first, last - 1, comp);
        if (pivot == kth)
            return;
        else if (pivot < kth)
            first = pivot + 1;
        else
            last = pivot;
    }
}

template <typename T, typename Fn>
double median(const T *arr, size_t n, Fn partition_fn) {
    // make a copy since nth_element will modify the array
    std::vector<T> copy(arr, arr + n);
    T *data = copy.data();
    size_t m = n / 2;
    partition_fn(data, data + m, data + n);
    // odd
    if (n % 2 == 1)
        return data[m];
    // even
    return (data[m] + *std::max_element(data, data + m)) / 2.0;
}

// for testing zero memory copying
static double median_sort(py::array_t<double> np_array) {
    // Request a buffer descriptor from the NumPy array
    py::buffer_info buf_info = np_array.request();

    // Pointer to the data in the array
    double *data = static_cast<double *>(buf_info.ptr);

    size_t n = buf_info.size;

    std::sort(data, data + n);  // modifies the input array

    if (n % 2 == 0)
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;

    return data[n / 2];
}

extern "C" void select_nth_unstable_rust(double *ptr, size_t len, size_t index);

static void select_nth_unstable_rs(double *first, double *nth, double *last) {
    select_nth_unstable_rust(first, last - first, nth - first);
}

static double median_select_nth_unstable_rust(py::array_t<double> np_array) {
    py::buffer_info buf_info = np_array.request();
    const double *data = static_cast<double *>(buf_info.ptr);
    size_t n = buf_info.size;

    return median(data, n, select_nth_unstable_rs);
}

static double median_nth_element(py::array_t<double> np_array) {
    py::buffer_info buf_info = np_array.request();
    const double *data = static_cast<double *>(buf_info.ptr);
    size_t n = buf_info.size;

    return median(data, n, std::nth_element<double *>);
}

static void quickselect_wrapper(double *first, double *nth, double *last) {
    quickselect(first, nth, last);
}

static double median_quickselect(py::array_t<double> np_array) {
    py::buffer_info buf_info = np_array.request();
    const double *data = static_cast<double *>(buf_info.ptr);
    size_t n = buf_info.size;

    // the signature of quickselect<double *> has 4 parameters (though the last one is optional)
    // return median(data, n, quickselect<double *>);
    return median(data, n, quickselect_wrapper);  // or use a lambda
}

PYBIND11_MODULE(algos, m) {
    const char *desc = "Compute the median of a numpy array";

    m.def("median_sort", &median_sort,
          "Compute the median of a numpy array (altering the input array)");
    m.def("median_nth_element", &median_nth_element, desc);
    m.def("median_quickselect", &median_quickselect, desc);
    m.def("median_select_nth_unstable_rust", &median_select_nth_unstable_rust, desc);
}
