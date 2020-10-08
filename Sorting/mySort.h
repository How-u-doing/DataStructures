/**
 *  see this file at following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/blob/master/Sorting/mySort.h
 */
#ifndef MYSORT_H
#define MYSORT_H
#include <functional> // std::less<T>
#include <utility>	  // std::move
#include <cstddef>	  // std::ptrdiff_t

namespace mySortingAlgo{

#ifdef NDEBUG
	const int ISORT_MAX = 32;	// maximum size for insertion sort
#else 
	const int ISORT_MAX = 4;	// for test
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

template<typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Insertion_sort>
	auto len = last - first;
	for (auto i = 1; i < len; ++i) {
		// insert a[i] into the sorted sequence a[0..i-1]
		for (auto j = i; j > 0 && comp(*(first + j), *(first + j - 1)); --j)
			iter_swap(first + j, first + j - 1);
	}
}

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
			if (M > first)	R = M - 1;
			else break;
		}
		else if (comp(*M, *target))
			L = M + 1;
		else { // *M == *target, to keep this insertion sort stable, we should insert target after M.
			   // Note that there may have more than 1 element that is equal to *M.
			auto it = M + 1;
			while (it != last && !comp(*M, *it) && !comp(*it, *M)) // *M == *it
				++it;
			return it;	// if it==last, insert target before last
		}
	}
	return L;
}

template<typename RandomIt, typename Compare>
void binary_insertion_sort(RandomIt first, RandomIt last, Compare comp)
{
	// When we have massive data items, we can save some comparison time by using binary search.
	for (auto it = first + 1; it != last; ++it) {
		auto pos = binary_insertion_search(first, it, it, comp);
		if (pos == it)	continue;
		// insert *it before (at) position 'pos'
		auto tmp = *it;
		for (auto iter = it; iter != pos; --iter)
			*iter = *(iter - 1);
		*pos = tmp;
	}
}

template<typename RandomIt, typename Compare>
void selection_sort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Selection_sort>
	for (auto i = first; i != last - 1; ++i) {
		auto jM = i;	// jMax or jMin
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
	//auto obj = *low;	// an object for decltype
	//auto b = new decltype(obj)[m] {};	  // auxiliary array for temporary storage and comparison
	auto b = new typename std::iterator_traits<RandomIt>::value_type[m]{};

	auto it = low;
	for (auto i = 0; i < m; ++i) {// copy a[lo..mid] to b[0..m-1]
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

template<typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Merge_sort>
	if (last - first < 2) return;
	auto mid = first + ((last - first - 1) >> 1);
	merge_sort(first, mid + 1, comp);	// sort a[lo..mid]
	merge_sort(mid + 1, last, comp);	// sort a[mid+1..hi]
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
		size_t j{};	// largest or smallest between {left, right}
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
		//
		// About the template argument deduction see
		// <https://en.cppreference.com/w/cpp/language/template_argument_deduction> &
		// <https://stackoverflow.com/questions/24277974/couldnt-deduce-template-parameter-from-function-parameters-default-argument>
		// When we want to deduce template arg for default arg,  set the template arg (i.e. type) default 
		// rather than function parameter, as there might be numerious template arg types can attain such
		// goal. e.g. Compare_type_1 = long,   comp = std::less<int>,
		//            Compare_type_2 = string, comp = std::less<int>, 
		//            ...	   (don't forget template specialization)
		// the compiler has so many choices to select, and it's getting confused!
		//
		//========================================================================================================

		auto n = last - first;	// number of nodes
		int i = (n - 1) >> 1;	// parent node of last leaf
		while (i >= 0) {
			myHeap::sift_down(i--, first, n, comp);
		}
	}

	template <typename RandIt, typename Compare>
	void sift_up(size_t pos, RandIt A, size_t n, Compare comp) {
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
		myHeap::sift_up(last - first - 1, first, last - first, comp);
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
}// namespace myHeap

template<typename RandomIt, typename Compare>
void heapsort(RandomIt first, RandomIt last, Compare comp)
{
	myHeap::heapsort(first, last, comp);
}


// make high = median of {low, mid, high}
template<typename RandomIt, typename Compare>
inline void mid3(RandomIt low, RandomIt high, Compare comp)
{
	auto mid = low + ((high - low) >> 1);
	if (comp(*mid, *low))  iter_swap(low, mid);
	if (comp(*high, *low)) iter_swap(low, high);
	if (comp(*mid, *high)) iter_swap(mid, high);
}

template<typename RandomIt, typename Compare>
RandomIt Lomuto_partition(RandomIt low, RandomIt high, Compare comp)
{	
	// See 'CLRS-3e' p171-172 (illustration, Fig7.1).
	// See also <https://en.wikipedia.org/wiki/quicksort#Lomuto_partition_scheme>.
	mid3(low, high, comp); // make high = median of {low, mid, high}
	auto pivot = *high;	   // choose the rightmost element as pivot
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
	mid3(low, high, comp);
	auto pivot = *(low + ((high - low) >> 1));
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

template<typename RandomIt, typename Compare>
void quicksort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/quicksort>
	if (last - first < 2) return;
	if (last - first <= ISORT_MAX) { // hybrid with insertion sort for speed
		insertion_sort(first, last, comp);
		return;
	}
#if defined QUICK3WAY_PARTITION
	// See also 'Algorithms-4e' p298-299. 3-way quicksort is based on Dutch National Flag algorithm 
	// (see <https://www.geeksforgeeks.org/sort-an-array-of-0s-1s-and-2s/>).
	// Dijkstra's solution is based on a single left-to-right pass through the array that maintains a 
	// pointer lt such that a[lo..lt-1] is less than v, a pointer gt such that a[gt+1..hi] is greater 
	// than v, and a pointer i such that a[lt..i-1] are equal to v, and a[i..gt] are not yet examined.
		
	auto lt = first, i = first + 1, gt = last - 1;
	auto pivot = *first;
	while (i <= gt) {
		if		(comp(*i, pivot)) iter_swap(lt++, i++);
		else if (comp(pivot, *i)) iter_swap(i, gt--);
		else					  ++i;
	}	// Now a[lo..lt-1] < pivot = a[lt..gt] < a[gt+1..hi].
	quicksort(first, lt, comp);		// sort a[lo..lt-1]
	quicksort(gt + 1, last, comp);	// sort a[gt+1..hi]

#elif defined FAST3WAY_PARTITION // by J. Bentley and D. McIlroy
	// See 'Algorithms-4e' p306 CREATIVE PROBLEMS 2.3.22
	// Note that this partitioning scheme does extra swaps for keys equal to the partitioning item's key,
	// while Quick3way does extra swaps for keys that are NOT equal to the partitioning item's key.
	// See also <https://www.geeksforgeeks.org/3-way-quicksort-dutch-national-flag/>

	int p = -1;          // p points to left last one that is equal to pivot
	auto q = last - 1;   // q points to right first one that is equal to pivot
	int i = -1;          // a[p+1..i-1] < pivot, a[lo..p]==pivot
	auto j = last - 1;   // a[j+1..q-1] > pivot, a[q..hi]==pivot
	auto pivot = *q; //*(last - 1)
	while (true) {
		// increment i until it encounters first one that is less than pivot
		while (comp(*(first + (++i)), pivot));

		// decrement j until it meets first one that is greater than pivot
		while (comp(pivot, *(--j)))
			if (j == first)
				break;

		// stop when iterators cross
		if (first + i >= j) break;

		// swap, so that smaller goes on left and greater goes on right 
		iter_swap(first + i, j);

		// move all same left occurrence of pivot to beginning of array
		if (!comp(*(first + i), pivot) && !comp(pivot, *(first + i))) {
			iter_swap(first + (++p), first + i);
		}

		// move all same right occurrence of pivot to end of array
		if (!comp(pivot, *j) && !comp(*j, pivot)) {
			iter_swap(j, --q);
		}
	}

	// move pivot element to its correct index
	if (comp(pivot, *(first + i)))	// *(first+i)>=pivot
		iter_swap(first + i, last - 1);

	// move all left same occurrences from beginning to adjacent to a[i]
	if (i >= 1)
		j = first + i - 1;
	else
		j = first;

	for (int k = 0; k <= p; ++k) {
		iter_swap(first + k, j);
		if (j > first)
			--j;
		else
			break;
	}

	// move all right same occurrences from end to adjacent to a[i]
	i = i + 1;
	for (auto k = last - 2; k >= q; --k, ++i)
		iter_swap(first + i, k);

	// sort the rest non-equal area on both sides
	quicksort(first, j + 1, comp);    // sort a[lo..j]
	quicksort(first + i, last, comp); // sort a[i..hi]

#elif defined LOMUTO_PARTITION
	auto pivot = Lomuto_partition(first, last - 1, comp);
	quicksort(first, pivot, comp);		// sort a[first..pivot-1]
	quicksort(pivot + 1, last, comp);	// sort a[pivot+1..last-1]
#else
	auto pivot = Hoare_partition(first, last - 1, comp);
	quicksort(first, pivot + 1, comp);	// sort a[frist..pivot]
	quicksort(pivot + 1, last, comp);	// sort a[pivot+1..last-1]
#endif // defined Quick3way_partition
}

template<typename RandomIt, typename Compare>
void shellsort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/shellsort>
	int len = last - first;
	int gap = len;
	while (gap > 1) {
		gap = gap / 3 + 1;	// gap by Donald E. Knuth
		for (int i = gap; i < len; i += gap) {
			// insert a[i] into the sorted sequence a[0..i-1] (every gap count 1)
			for (int j = i; j > 0 && comp(*(first + j), *(first + j - gap)); j -= gap) {
				iter_swap(first + j, first + j - gap);
			}
		}	
	}
}

template<typename RandomIt, typename Compare>
void bubble_sort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Bubble_sort#Optimizing_bubble_sort>
	int n = last - first;	// unsorted length
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

/**
 * See also <https://en.wikipedia.org/wiki/Cocktail_shaker_sort>
 * Example: list{2,3,4,5,1}, which would only need to go through one pass (indeed 1.5 pass, one
 * more left-to-rigth comparison) of cocktail sort to become sorted, but if using an ascending 
 * bubble sort would take four passes. However one cocktail sort pass should be counted as two 
 * bubble sort passes. Typically cocktail sort is less than two times faster than bubble sort.
 */
template<typename RandomIt, typename Compare>
void cocktail_shaker_sort(RandomIt first, RandomIt last, Compare comp)
{
	int n = last - first;
	int m = 1;
	int lastLeftSwappedIndex;	// index of last left-side sorted
	int lastRightSwappedIndex;	// index of first right-side sorted
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

		if (n == 0)	// no swap, no need to compare right-to-left back
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
template<typename RandomIt, typename Compare>
void comb_sort(RandomIt first, RandomIt last, Compare comp)
{
	int n = last - first;	// unsorted length
	int gap = n;
	auto shrink = 1.3;	// gap shrink factor 1.3 has been suggested
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

template<typename RandomIt, typename Compare>
void counting_sort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void bucket_sort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void radix_sort(RandomIt first, RandomIt last, Compare comp)
{
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
		bubble_sort(first, last, comp);
		break;
	case Mode::comb_sort:
		comb_sort(first, last, comp);
		break;
	case Mode::counting_sort:
		counting_sort(first, last, comp);
		break;
	case Mode::bucket_sort:
		bucket_sort(first, last, comp);
		break;
	case Mode::radix_sort:
		radix_sort(first, last, comp);
		break;
	default:
		break;
	}
}
/*
// This one has a drawback that cannot use lambda functions
template<class RandomIt, class U>
void sort(RandomIt first, RandomIt last, bool (*comp)(const U& a, const U& b), Mode mode = Mode::quicksort)
{
	// implementation...
}

// Or like this using only one template argument via iterator_traits
template<class RandomIt>
void sort(RandomIt first, RandomIt last, bool (*comp)(
	const typename std::iterator_traits<RandomIt>::value_type& a,
	const typename std::iterator_traits<RandomIt>::value_type& b), Mode mode = Mode::quicksort)
{
	// implementation...
}
*/

/**
 * Time complexity: O(nlog(k)). It's fast when k isn't very large.
 * See also https://en.wikipedia.org/wiki/Partial_sorting, and
 * https://www.geeksforgeeks.org/sort-vs-partial_sort-vs-nth_element-sort-in-c-stl/
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

}// namespace mySortingAlgo

#endif // !MYSORT_H
