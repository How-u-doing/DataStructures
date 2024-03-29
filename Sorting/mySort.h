/*
 *  A collection of sorting algorithms
 *  see this file at the following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/blob/master/Sorting/mySort.h
 */

#ifndef MYSORT_H
#define MYSORT_H 1

#include <vector>
#include <algorithm>  // std::max/min_element
#include <functional> // std::less<T>
#include <utility>    // std::move
#include <limits>     // std::numeric_limits<T>::max()
#include <cstddef>    // std::ptrdiff_t
#include <cmath>      // std::floor
#include <cstring>    // std::strcmp
#include <cassert>

namespace mySortingAlgo {

#ifdef NDEBUG
    const int ISORT_MAX = 32;   // maximum size for insertion sort
#else
    const int ISORT_MAX = 4;    // for test
#endif

// swap content of two objects that are pointed to
// by iterator (pointer) it_1 & it_2, respectively
template<typename Iter>
inline void iter_swap(Iter it_1, Iter it_2)
{
    auto tmp = std::move(*it_1);
    *it_1 = std::move(*it_2);
    *it_2 = std::move(tmp);
}

#if 0
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/Insertion_sort>
    auto len = last - first;
    for (auto i = 1; i < len; ++i) {
        // insert a[i] into the sorted sequence a[0..i-1]
        for (auto j = i; j > 0 && comp(*(first + j), *(first + j - 1)); --j)
            iter_swap(first + j, first + j - 1);
    }
}
#else
// optimized version: see https://godbolt.org/z/5YMn33Wa4
// 5x speedup on my Mac (M2 Pro, arm64), 10x speedup on x86_64
// It wouldn't matter much though, given the cardinality is usually small for isort ;-)
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    auto A = first;
    auto len = last - first;
    for (auto i = 1; i < len; ++i) {
        auto x = A[i];
        auto j = i;
        // put A[i] into its position by shifting elements instead of swapping
        for (; j > 0 && comp(x, A[j - 1]); --j)
            A[j] = A[j - 1];
        A[j] = x;
    }
}
#endif

// Helper function for binary insertion routine.
// Return the position where target should INSERT_BEFORE in scope [first, last)
// It's efficient when the distance of sorted area is long.
template<typename RandomIt, typename Compare>
RandomIt binary_insertion_search(RandomIt first, RandomIt last, RandomIt target, Compare comp)
{
    // see also <https://en.wikipedia.org/wiki/Binary_search_algorithm>
    RandomIt L{ first }, R{ last - 1 }, M{};
    while (L <= R) {
        M = L + ((R - L) >> 1);
        if (comp(*target, *M)) {
            if (M > first)    R = M - 1;
            else break;
        }
        else if (comp(*M, *target))
            L = M + 1;
        else { // *M == *target, to keep this insertion sort stable, we should insert target after M.
               // Note that there may have more than 1 element that is equal to *M.
            auto it = M + 1;
            while (it != last && !comp(*M, *it) && !comp(*it, *M)) // *M == *it
                ++it;
            return it;    // if it==last, insert target before last
        }
    }
    return L;
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void binary_insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // When we have massive data items, we can save some comparison time by using binary search.
    for (auto it = first + 1; it != last; ++it) {
        auto pos = binary_insertion_search(first, it, it, comp);
        if (pos == it)    continue;
        // insert *it before (at) position 'pos'
        auto tmp = *it;
        for (auto iter = it; iter != pos; --iter)
            *iter = *(iter - 1);
        *pos = tmp;
    }
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void selection_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/Selection_sort>
    for (auto i = first; i != last - 1; ++i) {
        auto jM = i;    // jMax or jMin
        for (auto j = i + 1; j != last; ++j) {
            if (comp(*j, *jM))
                jM = j;
        }
        if (jM != i)
            iter_swap(jM, i);
    }
}

// merge a[lo..mid] & a[mid+1..hi]
template<typename RandomIt, typename Compare>
void merge(RandomIt low, RandomIt mid, RandomIt high, Compare comp)
{
    auto m = mid - low + 1;
    //auto b = new decltype(*low)[m] {};  // error, *low is a reference, array of reference is not allowed
    //auto obj = *low;    // an object for decltype
    //auto b = new decltype(obj)[m] {};   // auxiliary array for temporary storage and comparison
    auto b = new typename std::iterator_traits<RandomIt>::value_type[m]{};

    auto it = low;
    for (auto i = 0; i < m; ++i) { // copy a[lo..mid] to b[0..m-1]
        b[i] = *it++;
    }

    std::ptrdiff_t i = 0, k = 0, j = m, n = high - low + 1;
    while (i < m && j < n) {
        *(low + k++) = comp(*(low + j), b[i]) ? *(low + j++) : b[i++];
    }

    while (i < m) {
        *(low + k++) = b[i++];
    }

    delete[] b;
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void merge_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/Merge_sort>
    if (last - first < 2) return;
    auto mid = first + ((last - first - 1) >> 1);
    merge_sort(first, mid + 1, comp);   // sort a[lo..mid]
    merge_sort(mid + 1, last, comp);    // sort a[mid+1..hi]
    merge(first, mid, last - 1, comp);
}

namespace myHeap {
    // implementation of stl-like func: make_heap, push_heap, etc.
    // note that it's simplified (no error handling)
    // see <https://en.cppreference.com/w/cpp/algorithm/push_heap>
    // see relative algorithms at CLRS-3e - heapsort
    // see also <https://en.wikipedia.org/wiki/heapsort>

    template <typename T>
    inline void swap(T& a, T& b) {
        T tmp = std::move(a);
        a = std::move(b);
        b = std::move(tmp);
    }

    template <typename RandIt, typename Compare>
    void sift_down(size_t pos, RandIt A, size_t n, Compare comp) {
        size_t j{};    // largest or smallest between {left, right}
        while ((j = (pos << 1) + 1) < n) {
            if (j + 1 < n && comp(*(A + j), *(A + j + 1))) ++j;
            if (comp(*(A + j), *(A + pos))) return;
            myHeap::swap(*(A + pos), *(A + j));
            pos = j;
        }
    }

    // Construct a heap in the range [first, last)
    template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
    void make_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
        //========================================================================================================
        // About the template argument deduction see
        // <https://en.cppreference.com/w/cpp/language/template_argument_deduction> &
        // <https://stackoverflow.com/questions/24277974/couldnt-deduce-template-parameter-from-function-parameters-default-argument>
        // When we want to deduce template arg for default arg,  set the template arg (i.e. type) default
        // rather than function parameter, as there might be numerious template arg types can attain such
        // goal. e.g. Compare_type_1 = long,   comp = std::less<int>,
        //            Compare_type_2 = string, comp = std::less<int>,
        //            ...       (don't forget template specialization)
        // the compiler has so many choices to select, and it's getting confused!
        //========================================================================================================
        auto n = last - first;   // number of nodes
        int i = (n - 1) >> 1;    // parent node of last leaf
        while (i >= 0) {
            myHeap::sift_down(i--, first, n, comp);
        }
    }

    template <typename RandIt, typename Compare>
    void sift_up(size_t pos, RandIt A, Compare comp) {
        size_t parent{};
        while (pos > 0 && comp(*(A + (parent = (pos - 1) >> 1)), *(A + pos))) {
            myHeap::swap(*(A + parent), *(A + pos));
            pos = parent;
        }
    }

    // Insert the element at the position last-1 into the
    // heap defined by the range [first, last-1)
    template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
    inline void push_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
        myHeap::sift_up(last - first - 1, first, comp);
    }

    // Swap the value in the pos first and the value in the pos
    // last-1 and make the subrange [first, last-1) into a heap
    template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
    inline void pop_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
        myHeap::swap(*first, *(last - 1));
        myHeap::sift_down(0, first, last - first - 1, comp);
    }

    // Convert the heap [first, last) into a sorted range in ascending/descending order
    template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
    void sort_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
        // move root node (the largest/smallest) to the end
        while (last - first > 1) {
            myHeap::pop_heap(first, last--, comp);
        }
    }

    template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
    void heapsort(RandIt first, RandIt last, Compare comp = Compare{}) {
        // build a heap
        myHeap::make_heap(first, last, comp);

        // move root node (the largest/smallest) to the end
        while (last - first > 1) {
            myHeap::pop_heap(first, last--, comp);
        }
    }
} // namespace myHeap

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void heapsort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    myHeap::heapsort(first, last, comp);
}


// make high = median of {low, mid, high}
template<typename RandomIt, typename Compare>
inline void mid3(RandomIt low, RandomIt high, Compare comp)
{
    auto mid = low + ((high - low) >> 1);
    if (comp(*mid,  *low)) iter_swap(low, mid);
    if (comp(*high, *low)) iter_swap(low, high);
    if (comp(*mid, *high)) iter_swap(mid, high);
}

template<typename RandomIt, typename Compare>
RandomIt Lomuto_partition(RandomIt low, RandomIt high, Compare comp)
{
    // See 'CLRS-3e' p171-172 (illustration, Fig7.1).
    // See also <https://en.wikipedia.org/wiki/quicksort#Lomuto_partition_scheme>.
    mid3(low, high, comp);  // make high = median of {low, mid, high}
    auto pivot = *high;     // choose the rightmost element as pivot
    auto i = low;
    for (auto j = low; j != high; ++j) {
        if (comp(*j, pivot))
            iter_swap(i++, j);
    }
    iter_swap(i, high);
    return i;
    // Now a[lo..i-1] <= pivot, *i==pivot, a[i+1..hi] >= pivot. (operator<, ascending)
    // Then quicksort a[lo..i-1] & a[i+1..hi].
}

template<typename RandomIt, typename Compare>
RandomIt Hoare_partition(RandomIt low, RandomIt high, Compare comp)
{
    // see also <https://en.wikipedia.org/wiki/quicksort#Hoare_partition_scheme>
    mid3(low, high, comp);    // make high = median of {low, mid, high}
    auto pivot = *high;       // choose the rightmost element as pivot
    // Note: here j can not be high-1 when there are only two elements because
    // if comp(pivot, *j) then j will get decreased again, thus pointing to an
    // invalid position and we will return it.
    auto i = low, j = high;
    while (true) {
        while (comp(*i, pivot)) ++i;
        while (comp(pivot, *j)) --j;
        if (i >= j) return j;
        iter_swap(i++, j--);
    }
    // Now a[lo..j] <= pivot, a[j+1..hi] >= pivot. (operator<, ascending)
    // Then quicksort a[lo..j] & a[j+1..hi].
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick3way(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    if (last - first < 2) return;
    if (last - first <= ISORT_MAX) { // hybridize with insertion sort for speed
        insertion_sort(first, last, comp);
        return;
    }
    // See also 'Algorithms-4e' p298-299. 3-way quicksort is based on Dutch National Flag algorithm
    // (see <https://en.wikipedia.org/wiki/Dutch_national_flag_problem>).
    // Dijkstra's solution is based on a single left-to-right pass through the array that maintains a
    // pointer lt such that a[lo..lt-1] is less than v, a pointer gt such that a[gt+1..hi] is greater
    // than v, and a pointer i such that a[lt..i-1] are equal to v, and a[i..gt] are not yet examined.
    // But we use a reversed version of Dijkstra's approach, given mid3() makes hi the midian :)
#if 0
    mid3(first, last - 1, comp);
    iter_swap(first, last - 1);  // or we can do one more swap
    auto lt = first, i = first + 1, gt = last - 1;
    auto pivot = *first;
    while (i <= gt) {
        if      (comp(*i, pivot)) iter_swap(lt++, i++);
        else if (comp(pivot, *i)) iter_swap(i, gt--);
        else                      ++i;
    }
#else
    mid3(first, last - 1, comp);
    auto lt = first, i = last - 2, gt = last - 1;
    auto pivot = *gt;
    while (i >= lt) {
        if      (comp(*i, pivot)) iter_swap(lt++, i);
        else if (comp(pivot, *i)) iter_swap(i--, gt--);
        else                      --i;
    }
#endif
    // Now a[lo..lt-1] < pivot = a[lt..gt] < a[gt+1..hi].
    quick3way(first, lt, comp);      // sort a[lo..lt-1]
    quick3way(gt + 1, last, comp);   // sort a[gt+1..hi]
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void fast3way(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    if (last - first < 2) return;
    if (last - first <= ISORT_MAX) { // hybridize with insertion sort for speed
        insertion_sort(first, last, comp);
        return;
    }
    // by J. Bentley and D. McIlroy
    // See 'Algorithms-4e' p306 CREATIVE PROBLEMS 2.3.22
    // Note that this partitioning scheme does extra swaps for keys equal to the partitioning item's key,
    // while Quick3way does extra swaps for keys that are NOT equal to the partitioning item's key.

    /*
     *          -----------------------------------------------
     *  before  |                                           |v|
     *          -----------------------------------------------
     *           ^                                           ^
     *           |                                           |
     *           lo                                          hi
     *
     *          -----------------------------------------------
     *  during  |   =v   |   <v   |         |   >v   |   =v   |
     *          -----------------------------------------------
     *           ^        ^        ^       ^        ^        ^
     *           |        |        |       |        |        |
     *           lo       p        i       j        q        hi
     *
     *          -----------------------------------------------
     *   after  |       <v      |     =v      |      >v       |
     *          -----------------------------------------------
     *           ^             ^               ^             ^
     *           |             |               |             |
     *           lo            j               i             hi
     */
    mid3(first, last - 1, comp);
    auto p = first;      // a[lo..p-1] = v
    auto q = last - 2;   // a[q+1..hi] = v
    auto i = first;      // a[p..i-1] < v
    auto j = last - 2;   // a[j+1..q] > v
    auto pivot = *(last-1); // v = a[hi]
    while (true) {
        while (comp(*i, pivot)) ++i;
        while (comp(pivot, *j)) --j;

        if (i >= j) break;

        iter_swap(i, j);

        // Change this to `if (*i == pivot)` if == comparison is available,
        // it can save one extra (potentially long) comparison (for strings).
        if (!comp(*i, pivot) && !comp(pivot, *i))
            iter_swap(p++, i);

        if (!comp(pivot, *j) && !comp(*j, pivot))
            iter_swap(j, q--);

        ++i; --j;
    }
    if (i == j) // a[i] == a[j] == pivot
        iter_swap(p++, i++);
    // Now a[lo..p-1] = v, a[p..j] < v,
    //     a[q+1..hi] = v, a[i..q] > v (i == j+1).
    // Note: p may be equal to lo (no =v on the lhs),
    // while at least one =v (the pivot) on the rhs.

    /* swap the items with equal keys into position */

    for (auto k = first; k != p; ++k)
        iter_swap(k, j--);

    for (auto k = last-1; k != q; --k)
        iter_swap(i++, k);

    // now a[lo..j] < v = a[j+1..i-1] < a[i..hi]
    fast3way(first, j+1, comp); // sort a[lo..j]
    fast3way(i, last, comp);    // sort a[i..hi]
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quicksort_lomuto(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    if (last - first < 2) return;
    if (last - first <= ISORT_MAX) { // hybridize with insertion sort for speed
        insertion_sort(first, last, comp);
        return;
    }
    auto pivot = Lomuto_partition(first, last - 1, comp);
    quicksort_lomuto(first, pivot, comp);       // sort a[first..pivot-1]
    quicksort_lomuto(pivot + 1, last, comp);    // sort a[pivot+1..last-1]
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quicksort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/quicksort>
#if defined(QUICK3WAY_PARTITION)
    quick3way(first, last, comp);
#elif defined(FAST3WAY_PARTITION)
    fast3way(first, last, comp);
#elif defined(LOMUTO_PARTITION)
    quicksort_lomuto(first, last, comp);
#else
    if (last - first < 2) return;
    if (last - first <= ISORT_MAX) { // hybridize with insertion sort for speed
        insertion_sort(first, last, comp);
        return;
    }
    auto pivot = Hoare_partition(first, last - 1, comp);
    quicksort(first, pivot + 1, comp);   // sort a[frist..pivot]
    quicksort(pivot + 1, last, comp);    // sort a[pivot+1..last-1]
#endif
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void shellsort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/shellsort>
    int len = last - first;
    int gap = len;
    while (gap > 1) {
        gap = gap / 3 + 1;   // suggested by Donald E. Knuth
        for (int i = gap; i < len; i += gap) {
            // insert a[i] into the sorted sequence a[0..i-1] (every gap count 1)
            for (int j = i; j > 0 && comp(*(first + j), *(first + j - gap)); j -= gap) {
                iter_swap(first + j, first + j - gap);
            }
        }
    }
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void bubble_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    // see also <https://en.wikipedia.org/wiki/Bubble_sort#Optimizing_bubble_sort>
    int n = last - first;    // unsorted length
    int lastSwappedIndex = 0;
    while (n > 1) {
        lastSwappedIndex = 0;
        for (int i = 1; i < n; ++i) {
            if (comp(*(first + i), *(first + i - 1))) {
                iter_swap(first + i, first + i - 1);
                lastSwappedIndex = i;
            }
        }
        n = lastSwappedIndex;
    }
}

/*
 * Example: list{2,3,4,5,1}, which would only need to go through one pass (indeed 1.5 pass, one
 * more left-to-rigth comparison) of cocktail sort to become sorted, but if using an ascending
 * bubble sort would take four passes. However one cocktail sort pass should be counted as two
 * bubble sort passes. Typically cocktail sort is less than two times faster than bubble sort.
 * See also <https://en.wikipedia.org/wiki/Cocktail_shaker_sort>
 */
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void cocktail_shaker_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    int n = last - first;
    int m = 1;
    int lastLeftSwappedIndex;    // index of last left-side sorted
    int lastRightSwappedIndex;   // index of first right-side sorted
    while (n > m) {
        lastLeftSwappedIndex = n - 1;
        lastRightSwappedIndex = 0;
        for (int i = m; i < n; ++i) {
            if (comp(*(first + i), *(first + i - 1))) {
                iter_swap(first + i, first + i - 1);
                lastRightSwappedIndex = i;
            }
        }
        n = lastRightSwappedIndex;

        if (n == 0)  // no swap, no need to compare right-to-left back
            return;

        for (int j = n - 1; j >= m; --j) {
            if (comp(*(first + j), *(first + j - 1))) {
                iter_swap(first + j, first + j - 1);
                lastLeftSwappedIndex = j;
            }
        }
        m = lastLeftSwappedIndex;
    }
}

// The main idea is like shellsort, but based on
// bubble sort (shellsort is based on insertion sort).
// See also <https://en.wikipedia.org/wiki/Comb_sort>
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void comb_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    int n = last - first; // unsorted length
    int gap = n;
    auto shrink = 1.3;    // gap shrink factor 1.3 has been suggested
    bool sorted = false;
    while (sorted == false) {
        gap = int(gap / shrink);
        if (gap < 1) {
            gap = 1;
            sorted = true;
        }
        for (int i = 0; i + gap < n; ++i) {
            if (comp(*(first + i + gap), *(first + i))) {
                iter_swap(first + i + gap, first + i);
                sorted = false;
            }
        }
    }
}

// An unstable sort with O(n + r) time complexity and O(r) space
// complexity, where r is the range of the non-negative key values.
// Works for string, vector<unsigned char/short>, etc.
template<typename RandomIt>
void counting_sort_unstable(RandomIt first, RandomIt last)
{
    typedef typename std::iterator_traits<RandomIt>::value_type Key;
    constexpr int r = std::numeric_limits<Key>::max() + 1; // radix
    static_assert(r > 0); // to avoid int or size_t type
    int freq[r]{ 0 };
    RandomIt it;
    int c;

    for (it = first; it != last; ++it) {
        assert(*it >= 0 && "pointed value must be non-negative"); // especially string
        ++freq[(unsigned) *it];
    }

    for (it = first, c = 0; c < r; ++c) {
        while (freq[c]-- > 0)
            *it++ = c;
    }
}

// A stable sort with O(n + r) time & space complexity,
// where r is the range of the non-negative key values.
template<typename RandomIt>
void counting_sort(RandomIt first, RandomIt last)
{
    typedef typename std::iterator_traits<RandomIt>::value_type Key;
    constexpr int r = std::numeric_limits<Key>::max() + 1; // radix
    static_assert(r > 0);  // to avoid int or size_t type
    const int n = last - first;
    int count[r + 1]{ 0 };
    Key* aux = new Key[n]; // vector or std::unique_ptr<Key[]> aux{ new Key[n] };

    for (RandomIt it = first; it != last; ++it) {
        assert(*it >= 0 && "pointed value must be non-negative");
        ++count[*it + 1];
    }

    // transform counts to indices
    for (int i = 0; i < r; ++i)
        count[i + 1] += count[i];

    // distribute a[i] to aux[j] (the right position)
    for (RandomIt i = first; i != last; ++i)
        aux[count[(unsigned) *i]++] = *i;

    // copy back
    for (int i = 0; i < n; ++i)
        *(first + i) = aux[i];

    delete[] aux;
}

// Extended counting sort that can take negative values.
// Works for string, vector<char>, vector<int>, etc.
template<typename RandomIt>
void counting_sort_ext(RandomIt first, RandomIt last)
{
    typedef typename std::iterator_traits<RandomIt>::value_type Key;
    const int max = *std::max_element(first, last);
    const int min = *std::min_element(first, last);
    const int r = max - min + 1; assert(r > 0);
    const int n = last - first;
    int* count = new int[r + 1]{ 0 };
    Key* aux = new Key[n];

    for (RandomIt it = first; it != last; ++it) {
        ++count[*it - min + 1];
    }

    // transform counts to indices
    for (int i = 0; i < r; ++i)
        count[i + 1] += count[i];

    // distribute a[i] to aux[j] (the right position)
    for (RandomIt i = first; i != last; ++i)
        aux[count[*i - min]++] = *i;

    // copy back
    for (int i = 0; i < n; ++i)
        *(first + i) = aux[i];

    delete[] aux;
    delete[] count;
}

template<typename RandomIt>
void counting_sort_ext_unstable(RandomIt first, RandomIt last)
{
    const int max = *std::max_element(first, last);
    const int min = *std::min_element(first, last);
    const int r = max - min + 1; assert(r > 0);
    int* freq = new int[r]{ 0 };
    RandomIt it;
    int c;

    for (it = first; it != last; ++it) {
        ++freq[*it - min];
    }

    for (it = first, c = 0; c < r; ++c) {
        while (freq[c]-- > 0)
            *it++ = c + min;
    }
    delete[] freq;
}

// Works for an array of numerics, e.g. vector<char/int/float/double>, etc.
template<typename RandomIt>
void bucket_sort(RandomIt first, RandomIt last, size_t bucket_size)
{
    typedef typename std::iterator_traits<RandomIt>::value_type Key;
    const Key max = *std::max_element(first, last);
    const Key min = *std::min_element(first, last);
    const Key M = max - min; assert(M > 0);
    const size_t bucket_count = std::floor(M / bucket_size) + 1;
    std::vector<std::vector<Key>> bucket_list(bucket_count);

    // distribute values into their corresponding buckets
    for (RandomIt it = first; it != last; ++it) {
        bucket_list[std::floor((*it - min) / bucket_size)].push_back(*it);
    }

    // sort & concatenate each bucket
    int sortedIdx = 0;
    for (size_t i = 0; i < bucket_count; ++i) {
        if (bucket_list[i].size() > 1)
            insertion_sort(bucket_list[i].begin(), bucket_list[i].end(), std::less<Key>());
        // concatenate
        for (auto x : bucket_list[i])
            *(first + sortedIdx++) = x;
    }
}

// for std::string, etc.
template<typename StringT>
struct string_traits
{
    typedef typename StringT::value_type value_type;
};

// for const char*, etc
template<typename CharT>
struct string_traits<CharT*>
{
    typedef std::remove_cv_t<CharT> value_type;
};

/*
 * LSD radix sort sorts an array of fixed-size strings with W characters.
 * Time complexity: O(N*W), space complexity: O(N+R).
 * Works for: vector<string>, string[] ;
 *            vector<[const] char*>, [const] char* [], etc.
 * Note that  char(*) [D + 1]  may look better than char* [] since it
 * points to an array of fixed-size (that is D + 1) C strings. But it
 * also means it's an array of const pointers which cannot be changed.
 * i.e. const char a[][5] = { "1234", "5678" };
 *      a[1] = "ABCD"; // Oops, no, a[1] is NOT a modifiable lvalue
 * But: const char* b[] = { "1234", "5678" };
 *      b[1] = "ABCD"; // Okay, b[1] is a modifiable lvalue
 * So, we DON'T support char(*)[D+1] as it would incur lots of copies.
 */
template<typename RandomIt>
void radix_sort(RandomIt first, RandomIt last, size_t W)
{
    typedef typename std::iterator_traits<RandomIt>::value_type String;
    typedef typename string_traits<String>::value_type CharT;
    typedef std::make_unsigned_t<CharT> UCharT;
    /*
     * On Linux, sizeof( wchar_t ) is 4, while 2 on Windows. So, here
     * wstring on Linux might incur integer overflow, thus Radix == 0.
     * Best practice is to almost never use wstring on Linux and almost
     * alway use it on Windows. See more detail at
     * https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring/402918#402918
     */
    constexpr int Radix = std::numeric_limits<UCharT>::max() + 1;
    static_assert(Radix > 0);
    const int n = last - first;
    auto a = & *first; // raw data of the arr, of type String*
    std::vector<String> aux(n);

    // sort the dth character by counting sort
    for (int d = W - 1; d >= 0; --d) {
        int count[Radix + 1]{ 0 };

        // count frequencies
        for (int i = 0; i < n; ++i)
            ++count[(UCharT) a[i][d] + 1];

        // transform counts to indices
        for (int r = 0; r < Radix; ++r)
            count[r + 1] += count[r];

        // distribute
        for (int i = 0; i < n; ++i)
            aux[count[(UCharT) a[i][d]]++] = std::move(a[i]);

        // copy back
        for (int i = 0; i < n; ++i)
            a[i] = std::move(aux[i]);
    }
}

template<typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last, size_t d)
{
    const int len = last - first;
    for (int i = 1; i < len; ++i) {
        // insert a[i] into the sorted sequence a[0..i-1]
        for (int j = i; j > 0 && std::strcmp(&(*(first+j))[d], &(*(first+j-1))[d]) < 0; --j)
            iter_swap(first + j, first + j - 1);
    }
}

template<typename RandomIt>
void quick3string(RandomIt first, RandomIt last, size_t d)
{
    if (last - first < 2) return;
#if 0 // seems not to help much
    if (last - first <= 8) { // change the threshold as you like
        insertion_sort(first, last, d);
        return;
    }
#endif
    typedef typename std::iterator_traits<RandomIt>::value_type String;
    typedef typename string_traits<String>::value_type CharT;
    typedef std::make_unsigned_t<CharT> UCharT;

    RandomIt lt = first, i = first + 1, gt = last - 1;
    /* make lo = median of {lo, mid, hi} */
    RandomIt mid = lt + ((gt - lt) >> 1);
    if ((*mid)[d] < (*lt)[d]) iter_swap(lt, mid);
    if ((*mid)[d] < (*gt)[d]) iter_swap(gt, mid);
    // now mid is the largest of the three, then make lo the median
    if ((*lt)[d] < (*gt)[d]) iter_swap(lt, gt);

    UCharT pivot = (*first)[d];
    while (i <= gt) {
        int diff = (UCharT) (*i)[d] - pivot;
        if      (diff < 0) iter_swap(lt++, i++);
        else if (diff > 0) iter_swap(i, gt--);
        else               ++i;
    }
    // Now a[lo..lt-1] < pivot = a[lt..gt] < a[gt+1..hi].
    quick3string(first, lt, d);      // sort a[lo..lt-1]
    if (pivot != '\0')
        quick3string(lt, gt+1, d+1); // sort a[lt..gt] on following character
    quick3string(gt+1, last, d);     // sort a[gt+1..hi]
}

// MSD sort sorts subarrays whose first d characters
// are equal, starting at the dth character.
template<typename RandomIt>
void MSD_sort(RandomIt first, RandomIt last, size_t d)
{
    const int n = last - first;
    if (n < 2) return;
    /* For a small number of strings using MSD radix sort can be overkill,
     * as it would create lots of (empty) subarrays. Besides, long common
     * prefixes make MSD sort suffer as well. Hence, again, we apply a
     * hybrid solution, with insertion sort, like we did in quicksort.
     */
#if 1
    if (n <= 8) { // change the threshold as you like
        insertion_sort(first, last, d);
        return;
    }
#endif
    typedef typename std::iterator_traits<RandomIt>::value_type String;
    typedef typename string_traits<String>::value_type CharT;
    typedef std::make_unsigned_t<CharT> UCharT;
    constexpr int Radix = std::numeric_limits<UCharT>::max() + 1;
    static_assert(Radix > 0);
    auto a = & *first; // raw data of the arr, of type String*
    std::vector<String> aux(n);

#if 0  // compare this version with LSD radix sort
    int count[Radix + 1]{ 0 };

    /* sort the dth character by counting sort */
    // count frequencies
    for (int i = 0; i < n; ++i)
        ++count[(UCharT) a[i][d] + 1];
    // transform counts to indices
    for (int r = 0; r < Radix; ++r)
        count[r + 1] += count[r];
    // distribute
    for (int i = 0; i < n; ++i)
        aux[count[(UCharT) a[i][d]]++] = std::move(a[i]);
    // copy back
    for (int i = 0; i < n; ++i)
        a[i] = std::move(aux[i]);

    /* sort subarrays recursively */
    // Note that all non-zero values of count arrary have increased to the next
    // value in the distribution step above. Therefore, count[0] can be greater
    // than zero. The solution to avoid the special case below is to make a copy
    // of count array immediately after transforming counts to indices (before
    // the distribution step) or make a new array = [0, count_arr], or make count
    // array of size Radix+2 to leave one more space in advance, thus leading to
    // the next version.
    if (count[0] > 1 && a[count[0]][d] != '\0')
        MSD_sort(first, first + count[0], d+1);
    for (int r = 0; r < Radix - 1; ++r)
        if (count[r+1] - count[r] > 1 && a[count[r]][d] != '\0')
            MSD_sort(first + count[r], first + count[r+1], d+1);
#else
    int count[Radix + 2]{ 0 };

    /* sort the dth character by counting sort */
    // count frequencies
    for (int i = 0; i < n; ++i)
        ++count[(UCharT) a[i][d] + 2];
    // transform counts to indices
    for (int r = 0; r <= Radix; ++r)
        count[r + 1] += count[r];
    // distribute
    for (int i = 0; i < n; ++i)
        aux[count[(UCharT) a[i][d] + 1]++] = std::move(a[i]);
    // copy back
    for (int i = 0; i < n; ++i)
        a[i] = std::move(aux[i]);

    /* sort subarrays recursively */
    for (int r = 0; r < Radix; ++r)
        if (count[r+1] - count[r] > 1 && a[count[r]][d] != '\0')
#ifndef USE_MSD_PLUS_Q3S
            MSD_sort(first + count[r], first + count[r+1], d+1);
#else
            // MSD+Q3S (or rather multiway partitioning 3-way string quicksort)
            // outperforms both MSD and quick3way in our benchmark to sort the
            // text file leipzig1M.txt.
            quick3string(first + count[r], first + count[r+1], d+1);
#endif
#endif
}

/*
 * MSD (general-purpose) radix sort, which can handle variable-length strings.
 * Time complexity: O(N~N*w), space complexity: O(N+W*R).
 * Works for: vector<string>, string[] ;
 *            vector<[const] char*>, [const] char* [], etc.
 * NOTE: strings must be null-terminated. In C++, std::string operator[] with
 * pos == size() is well defined so that it always returns a reference to the
 * character with value CharT() (the null character '\0'). And we rely on it
 * in our implementation for proper termination. The C strings, for example,
 * "abcd", implicitly contains a '\0' character in the end. That's great, but
 * we cannot handle this: char arr[] = {'a','b','c','d'}; It's not null-ended
 * (has exactly 4 characters, whereas "abcd" has 5 characters) and it's very
 * dangerous (though it's valid) in general use.
 */
template<typename RandomIt>
void radix_sort(RandomIt first, RandomIt last)
{
    // recursively sort subarrarys
    MSD_sort(first, last, 0);
}

/*
 * Three-way string quicksort.
 * Similar to MSD radix sort, we first sort the array on the leading character
 * (using quicksort), then apply this method recursively on the subarrays. On
 * first sorting, a pivot v is chosen, then partition it in 3 parts, strings
 * whose first character are less than v, equal to v, and greater than v. Just
 * like the partitioning in classic quicksort but with comparing only the 1st
 * character instead of the whole string. After partitioning, only the middle
 * (equal-to-v) part can sort on the following character (index of d+1). The
 * other two recursively sort on the same depth (index of d) because these two
 * haven't been sorted on the dth character (just partitioned them: <v or >v).
 *
 * Time complexity: O(N~N*log(N)), space complexity: O(log(N)).
 * Explanation: N * string length (for partitioning, find equal-to-v part) +
 *              O(N*log(N)) (to do the quicksort thing)
 * character comparisons (instead of string comparisons in normal quicksort).
 */
template<typename RandomIt>
void str_qsort(RandomIt first, RandomIt last)
{
    quick3string(first, last, 0);
}

/* Some popular sorting algorithms */
enum class Mode
{
    // simple sorts
    insertion_sort, selection_sort,

    // efficient sorts
    merge_sort, heapsort, quicksort, shellsort,

    // bubble sort and variants
    bubble_sort, comb_sort,

    // distribution sort
    counting_sort, bucket_sort, radix_sort
};

/* Sort an array (random access) ranging [first, last) */
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void sort(RandomIt first, RandomIt last, Compare comp = Compare{}, Mode mode = Mode::quicksort)
{
    switch (mode)
    {
    case Mode::insertion_sort:
        insertion_sort(first, last, comp);
        break;
    case Mode::selection_sort:
        selection_sort(first, last, comp);
        break;
    case Mode::merge_sort:
        merge_sort(first, last, comp);
        break;
    case Mode::heapsort:
        heapsort(first, last, comp);
        break;
    case Mode::quicksort:
        quicksort(first, last, comp);
        break;
    case Mode::shellsort:
        shellsort(first, last, comp);
        break;
    case Mode::bubble_sort:
        //bubble_sort(first, last, comp);
        cocktail_shaker_sort(first, last, comp);
        break;
    case Mode::comb_sort:
        comb_sort(first, last, comp);
        break;

    // distribution sorts apply only to some particular types
    // call them separately according to the given data type
    case Mode::counting_sort:
    case Mode::bucket_sort:
    case Mode::radix_sort:
        break;
    default:
        break;
    }
}

#if 0
// This one has a drawback that it cannot use lambda functions
template<class RandomIt, class U>
void sort(RandomIt first, RandomIt last, bool (*comp)(const U& a, const U& b), Mode mode = Mode::quicksort)
{
    // implementation...
}

// Or like this use only one template parameter then compare via iterator_traits
template<class RandomIt>
void sort(RandomIt first, RandomIt last, bool (*comp)(
    const typename std::iterator_traits<RandomIt>::value_type& a,
    const typename std::iterator_traits<RandomIt>::value_type& b), Mode mode = Mode::quicksort)
{
    // implementation...
}
#endif


/*
 * Time complexity: O(k+(n-k)log(k)+klog(k)). It's fast when k isn't very large.
 * See also https://en.wikipedia.org/wiki/Partial_sorting
 * Alternative implementations:
 *
 * 1. Partial heapsort: Build a heap containing all n elements and then
 *    perform pop_heap() k times, giving time complexity O(n+klog(n)).
 *
 * 2. Use selection algorithms (say quickselect) to partition the array,
 *    and then sort these k smallest (largest) elements, at a total cost
 *    of O(n+klog(k)) operations.
 *
 * 3. Specialized partial sorting algorithms based on mergesort and quicksort,
 *    with expected time O(n+klog(k)).
 */
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void partial_sort(RandomIt first, RandomIt middle, RandomIt last, Compare comp = Compare{})
{
    myHeap::make_heap(first, middle, comp);
    auto k = middle - first;
    for (auto i = middle; i != last; ++i) {
        if (comp(*i, *first)) {
            iter_swap(i, first);
            myHeap::sift_down(0, first, k, comp);
        }
    }
    myHeap::sort_heap(first, middle, comp);
}

// same effect as stl nth_element
// see also https://en.wikipedia.org/wiki/Quickselect and
// https://en.cppreference.com/w/cpp/algorithm/nth_element
template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quickselect(RandomIt first, RandomIt kth, RandomIt last, Compare comp = Compare{})
{
    while (true) {
        if (last - first < 2) return;
        auto pivot = Lomuto_partition(first, last - 1, comp);
        if (pivot == kth) return;
        else if (pivot < kth) first = pivot + 1;
        else last = pivot;
    }
}

template<typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void partial_quicksort(RandomIt first, RandomIt kth, RandomIt last, Compare comp = Compare{})
{
    if (last - first < 2) return;
    auto pivot = Hoare_partition(first, last - 1, comp);
    partial_quicksort(first, kth, pivot + 1, comp);
    /*auto pivot = Lomuto_partition(first, last - 1, comp);
    partial_quicksort(first, kth, pivot, comp);*/
    if (pivot < kth - 1)
        partial_quicksort(pivot + 1, kth, last, comp);
}

} // namespace mySortingAlgo

#endif // !MYSORT_H
