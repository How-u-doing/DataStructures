#pragma once
#ifndef MYSORT_H
#define MYSOER_H
#include <functional> // std::less<T>
#include <utility>	  // std::move

namespace mySortingAlgo{

	const int ISORT_MAX = 4;	// maximum size for insertion sort, 32 will be appropriate. For test, we set it 4 here

/* Some popular sorting algorithms */
enum class Mode
{
	// simple sorts
	InsertionSort, SelectionSort,

	// efficient sorts
	MergeSort, Heapsort, Quicksort, Shellsort,

	// bubble sort and variants
	BubbleSort, CombSort,

	// distribution sort
	CountingSort, BucketSort, RadixSort

};

template<typename RandomIt, typename Compare>
class sortingMethods {
public:
	static void InsertionSort(RandomIt first, RandomIt last, Compare comp);
	// auxiliary function for insertion sort
	static RandomIt binary_search(RandomIt first, RandomIt last, RandomIt target, Compare comp);

	static void SelectionSort(RandomIt first, RandomIt last, Compare comp);
	static void MergeSort(RandomIt first, RandomIt last, Compare comp);
	// auxiliary function for merge sort. merge a[lo..mid] & a[mid+1..hi]
	static void merge(RandomIt low, RandomIt mid, RandomIt high, Compare comp);

	static void Heapsort(RandomIt first, RandomIt last, Compare comp);
	static void Quicksort(RandomIt first, RandomIt last, Compare comp);
	// auxiliary function for quicksort
	static RandomIt partition(RandomIt low, RandomIt high, Compare comp);

	static void Shellsort(RandomIt first, RandomIt last, Compare comp);
	static void BubbleSort(RandomIt first, RandomIt last, Compare comp);
	static void CombSort(RandomIt first, RandomIt last, Compare comp);
	static void CountingSort(RandomIt first, RandomIt last, Compare comp);
	static void BucketSort(RandomIt first, RandomIt last, Compare comp);
	static void RadixSort(RandomIt first, RandomIt last, Compare comp);

	static inline void swap(RandomIt it_1, RandomIt it_2);
};

/* Sort an array (or a list) ranging [first, last) */ 
template<typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp, Mode mode = Mode::Quicksort)
{
	switch (mode)
	{
	case Mode::InsertionSort:
		sortingMethods<RandomIt, Compare>::InsertionSort(first, last, comp);
		break;
	case Mode::SelectionSort:
		sortingMethods<RandomIt, Compare>::SelectionSort(first, last, comp);
		break;
	case Mode::MergeSort:
		sortingMethods<RandomIt, Compare>::MergeSort(first, last, comp);
		break;
	case Mode::Heapsort:
		sortingMethods<RandomIt, Compare>::Heapsort(first, last, comp);
		break;
	case Mode::Quicksort:
		sortingMethods<RandomIt, Compare>::Quicksort(first, last, comp);
		break;
	case Mode::Shellsort:
		sortingMethods<RandomIt, Compare>::Shellsort(first, last, comp);
		break;
	case Mode::BubbleSort:
		sortingMethods<RandomIt, Compare>::BubbleSort(first, last, comp);
		break;
	case Mode::CombSort:
		sortingMethods<RandomIt, Compare>::CombSort(first, last, comp);
		break;
	case Mode::CountingSort:
		sortingMethods<RandomIt, Compare>::CountingSort(first, last, comp);
		break;
	case Mode::BucketSort:
		sortingMethods<RandomIt, Compare>::BucketSort(first, last, comp);
		break;
	case Mode::RadixSort:
		sortingMethods<RandomIt, Compare>::RadixSort(first, last, comp);
		break;
	default:
		break;
	}
}

//// This one has a drawback that cannot call lambda function
//template<class RandomIt, class U>
//void sort(RandomIt first, RandomIt last, bool (*comp)(const U& a, const U& b), Mode mode = Mode::Quicksort)
//{
//	// implementation...
//}
//// Or like this using only one template argument via decltype
//template<class RandomIt>
//void sort(RandomIt first, RandomIt last, bool (*comp)(const decltype(*first)& a, const decltype(*first)& b), Mode mode = Mode::Quicksort)
//{
//	// implementation...
//}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::InsertionSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Insertion_sort>
#if !defined BINARY_INSERTION_SORT	// DIRECT_INSERTION_SORT
	// direct insertion sort
	int len = last - first;
	for (int i = 1; i < len; ++i) {
		// insert a[i] into the sorted sequence a[0..i-1]
		for (int j = i; j > 0 && comp(*(first + j), *(first + j - 1)); --j) {
			swap(first + j, first + j - 1);
		}
	}
#else	// BINARY_INSERTION_SORT
	// When we have massive data items, we can save some comparison time by using binary search.
	for (auto it = first + 1; it != last; ++it) {
		auto pos = binary_search(first, it, it, comp);
		if (pos == it)
			continue;

		// insert *it before (at) position 'pos', move by block backwardly
		auto tmp = *it;
		for (auto iter = it; iter != pos; --iter)
			*iter = *(iter - 1);
		*pos = tmp;
	}

#endif // !defined BINARY_INSERTION_SORT
}

// return the position where target should INSERT_BEFORE in scope [first, last)
template<typename RandomIt, typename Compare>
RandomIt sortingMethods<RandomIt, Compare>::binary_search(RandomIt first, RandomIt last, RandomIt target, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Binary_search_algorithm>
	RandomIt L{ first }, R{ last - 1 }, M{ }; 
	while (L <= R) {
		M = L + ((R - L) >> 1);	// M=L+(R-L)/2;
		if (comp(*target, *M)) {
			if (M > first)
				R = M - 1;	// Beautiful. Two cases - ascending or descending (operator< or operator>), but with uniform expression.
			else
				break;
		}
		else if (comp(*M, *target))
			L = M + 1;
		else { // *M == *target, to keep this insertion sort stable, we should insert target after M.
			   // Note that there may have more than 1 element that is equal to *M.
			auto it{ M + 1 };
			while (it != last && !comp(*M, *it) && !comp(*it, *M)) // *M == *it
				++it;

			return it;	// if it==last, insert target before last (after last-1)
		}
	}

	return L;
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::SelectionSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Selection_sort>
	for (auto i = first; i != last - 1; ++i) {
		auto jM = i;	// jMax or jMin
		for (auto j = i + 1; j != last; ++j) {
			if (comp(*j, *jM))
				jM = j;
		}
		if (jM != i)
			swap(jM, i);
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::MergeSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Merge_sort>
	if (last - first < 2) return;
	auto mid = first + ((last - 1 - first) >> 1);
	MergeSort(first, mid + 1, comp);	// sort a[lo..mid]
	MergeSort(mid + 1, last, comp);		// sort a[mid+1..hi]
	merge(first, mid, last - 1, comp);
}

// merge a[lo..mid] & a[mid+1..hi]
template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::merge(RandomIt low, RandomIt mid, RandomIt high, Compare comp)
{
	int m = mid - low + 1;
	//auto b = new decltype(*low)[m] {};	// error, *low is a reference, array of reference is not allowed
	//auto obj = *low;	// an object for decltype
	//auto b = new decltype(obj)[m] {};	// auxiliary array for temporary storage and comparison
	auto b = new typename std::iterator_traits<RandomIt>::value_type[m]{};

	auto it = low;
	for (int i = 0; i < m; ++i) {// copy a[lo..mid] to b[0..m-1]
		b[i] = *it++;
	}

	int i = 0, k = 0, j = m, n = high - low + 1;
	while (i < m && j < n) {
		*(low + k++) = comp(*(low + j), b[i]) ? *(low + j++) : b[i++];
	}

	while (i < m) {
		*(low + k++) = b[i++];
	}

	delete[] b;
}


namespace myHeap {
	// implementation of stl-like func: make_heap, push_heap, etc. 
	// note that it's simplified (no error handling)
	// see <https://en.cppreference.com/w/cpp/algorithm/push_heap>
	// see relative algorithms at CLRS-3e - Heapsort
	// see also <https://en.wikipedia.org/wiki/Heapsort>

	template <typename T>
	inline void swap(T& a, T& b) {
		T tmp{ std::move(a) };
		a = std::move(b);
		b = std::move(tmp);
	}

	template <typename RandIt, typename Compare>
	void sift_down(size_t pos, RandIt A, size_t n, Compare comp) {
		auto left = 2 * pos + 1;
		size_t LS{ pos };	// largest or smallest among {parent, left, right}
		while (left < n) {
			if (comp(*(A + pos), *(A + left)))
				LS = left;
			if (left + 1 < n && comp(*(A + LS), *(A + left + 1))) // has right child
				LS = left + 1;

			if (LS == pos)
				return;
			else {
				myHeap::swap(*(A + pos), *(A + LS));
				pos = LS;
				left = 2 * pos + 1;
			}
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

		auto n{ last - first };	// number of nodes
		auto i{ (n - 1) >> 1 }; // parent node of last leaf
		while (i >= 0) {
			myHeap::sift_down(i--, first, n, comp);
		}
	}

	template <typename RandIt, typename Compare>
	void sift_up(size_t pos, RandIt A, size_t n, Compare comp) {
		if (pos == 0) return;
		size_t parent = (pos - 1) >> 1;
		while (pos > 0) {
			if (comp(*(A + parent), *(A + pos))) {
				myHeap::swap(*(A + parent), *(A + pos));
				pos = parent;
				parent = (pos - 1) >> 1;
			}
			else
				return;
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
void sortingMethods<RandomIt, Compare>::Heapsort(RandomIt first, RandomIt last, Compare comp)
{
	myHeap::heapsort(first, last, comp);
}

// quicksort elements in [first, last-1]
template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Quicksort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Quicksort>
	// Define your partition scheme  before including this header,
	// e.g. #define Median3_partition \NewLine #include "mySort.h".

	if (last - first < 2) return;
//#define QUICK_INSERTION_SORT	// uncomment this to set it default for performance
#if defined QUICK_INSERTION_SORT
	if (last - first <= ISORT_MAX) { // hybrid with insertion sort
		sortingMethods::InsertionSort(first, last, comp);
		return;
	}
#endif
#if defined Quick3way_partition
	// see also 'Algorithms-4e' p298-299. 3-way Quicksort is based on Dutch National Flag algorithm 
	// (see also <https://www.geeksforgeeks.org/sort-an-array-of-0s-1s-and-2s/>).
	// Dijkstra's solution is based on a single left-to-right pass through the array that maintains a 
	// pointer lt such that a[lo..lt-1] is less than v, a pointer gt such that a[gt+1..hi] is greater 
	// than v, and a pointer i such that a[lt..i-1] are equal to v, and a[i..gt] are not yet examined.
		
	auto lt = first, i = first + 1, gt = last - 1;
	auto pivot = *first;
	while (i <= gt) {
		if (comp(*i, pivot)) {
			swap(lt++, i++);
		}
		else if (comp(pivot, *i)) {
			swap(i, gt--);
		}
		else	++i;
	}	// Now a[lo..lt-1] < pivot = a[lt..gt] < a[gt+1..hi].
	Quicksort(first, lt, comp);		// sort a[lo..lt-1]
	Quicksort(gt + 1, last, comp);	// sort a[gt+1..hi]

#elif defined Fast3way_partition // by J. Bentley and D. McIlroy
	// See also 'Algorithms-4e' p306 CREATIVE PROBLEMS 2.3.22
	// Note that this partitioning scheme does extra swaps for keys equal to the partitioning item's key,
	// while Quick3way does extra swaps for keys that are NOT equal to the partitioning item's key.

	// see also <https://www.geeksforgeeks.org/3-way-quicksort-dutch-national-flag/>
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
		swap(first + i, j);

		// move all same left occurrence of pivot to beginning of array
		if (!comp(*(first + i), pivot) && !comp(pivot, *(first + i))) {
			swap(first + (++p), first + i);
		}

		// move all same right occurrence of pivot to end of array
		if (!comp(pivot, *j) && !comp(*j, pivot)) {
			swap(j, --q);
		}
	}

	// move pivot element to its correct index
	if (comp(pivot, *(first + i)))	// *(first+i)>=pivot
		swap(first + i, last - 1);

	// move all left same occurrences from beginning to adjacent to a[i]
	if (i >= 1)
		j = first + i - 1;
	else
		j = first;

	for (int k = 0; k <= p; ++k) {
		swap(first + k, j);
		if (j > first)
			--j;
		else
			break;
	}

	// move all right same occurrences from end to adjacent to a[i]
	i = i + 1;
	for (auto k = last - 2; k >= q; --k, ++i)
		swap(first + i, k);

	// sort the rest non-equal area on both sides
	Quicksort(first, j + 1, comp);    // sort a[lo..j]
	Quicksort(first + i, last, comp); // sort a[i..hi]

#endif // defined QUICK_INSERTION_SORT

// Lomuto_partition, Median3_partition, or Hoare_partition
#if defined Lomuto_partition || defined Median3_partition
	auto pivot = partition(first, last - 1, comp);
	Quicksort(first, pivot, comp);		// sort a[first, pivot-1]
	Quicksort(pivot + 1, last, comp);	// sort a[pivot+1, last-1]
#elif defined Hoare_partition
	auto pivot = partition(first, last - 1, comp);
	Quicksort(first, pivot + 1, comp);	// sort a[frist, pivot]
	Quicksort(pivot + 1, last, comp);	// sort a[pivot+1, last-1]
#endif // defined Lomuto_partition || defined Median3_partition

}

// partition ranging [left, right]
template<typename RandomIt, typename Compare>
RandomIt sortingMethods<RandomIt, Compare>::partition(RandomIt low, RandomIt high, Compare comp)
{
#if defined Lomuto_partition || defined Median3_partition
#if defined Median3_partition	// Median-of-3 partition
	// Choose the median of the first, middle, and last element of the partition for the pivot.
	// See also <https://en.wikipedia.org/wiki/Quicksort#Choice_of_pivot>.
	auto mid = low + ((high - low) >> 1);
	if (comp(*mid, *low)) { // after this, *low <= *mid (operator<) or *low >= *mid (operator>)
		swap(low, mid);
	}
	if (comp(*high, *low)) { // after this, *low <= *high (operator<) or *low >= *high (operator>)
		swap(low, high);
	}
	if (comp(*mid, *high)) { // insure the median is at the rightmost position
		swap(mid, high);
	}
	// Now *high is the median of *low, *mid, and *high.

#endif
	// See 'CLRS-3e' p171-172 (illustration, Fig7.1).
	// We can also choose a random iterator in [lo, hi] as pivot (i.e. k=RANDOM(lo,hi)), then swap(k, hi) if k!=hi.
	// See also <https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme>.
	auto pivot = *high;	 // choose the rightmost element as pivot
	auto i = low;
	for (auto j = low; j != high; ++j) {
		if (comp(*j, pivot)) {
			if (i != j)
				swap(i, j);
			++i;
		}
	}
	swap(i, high);
	return i;
	// Now [lo..i-1] <= pivot, *i==pivot, [i+1..hi] >= pivot. (operator<, ascending)
	// Then quicksort [lo..i-1] & [i+1..hi] respectively.

#elif defined Hoare_partition
	// see also <https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme>
	auto pivot = *(low + ((high - low) >> 1));	// pivot=*(low+(high-low)/2);
	auto i = low, j = high;
	while (true) {
		while (comp(*i, pivot)) {
			++i;
		}
		while (comp(pivot, *j)) {
			--j;
		}
		if (i >= j) {	// possibly j=i-1
			return j;
		}
		swap(i++, j--);
	}

	// After return, [lo..j] <= pivot, [j+1..hi] >= pivot. (operator<, ascending)
	// You can imagine that &pivot < i = j-1, *i>pivot, *j<pivot. The follow-up is to 
	// swap(i,j), ++i & --j such that j=i-1, [low..j] <= pivot, [j..high] >= pivot.
	// Afterward, what we need to do is to quicksort [lo..j] & [j+1..hi] respectively.

#endif // defined Lomuto_partition || defined Median3_partition	
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Shellsort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Shellsort>
	int len = last - first;
	int gap = len;
	while (gap > 1) {
		gap = gap / 3 + 1;	// gap by Donald E. Knuth
		for (int i = gap; i < len; i += gap) {
			// insert a[i] into the sorted sequence a[0..i-1] (every gap count 1)
			for (int j = i; j > 0 && comp(*(first + j), *(first + j - gap)); j -= gap) {
				swap(first + j, first + j - gap);
			}
		}	
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::BubbleSort(RandomIt first, RandomIt last, Compare comp)
{	
#if defined Cocktail_shaker_sort
	// see also <https://en.wikipedia.org/wiki/Cocktail_shaker_sort>
	// Example: list (2,3,4,5,1), which would only need to go through one pass (indeed 1.5 pass, one
	// more left-to-rigth comparison) of cocktail sort to become sorted, but if using an ascending 
	// bubble sort would take four passes. However one cocktail sort pass should be counted as two 
	// bubble sort passes. Typically cocktail sort is less than two times faster than bubble sort.
	int n = last - first;
	int m = 1;
	int lastLeftSwappedIndex;	// index of last left-side sorted
	int lastRightSwappedIndex;	// index of first right-side sorted
	while (n > m) {
		lastLeftSwappedIndex = n - 1;
		lastRightSwappedIndex = 0;
		for (int i = m; i < n; ++i) {
			if (comp(*(first + i), *(first + i - 1))) {
				swap(first + i, first + i - 1);
				lastRightSwappedIndex = i;
			}
		}
		n = lastRightSwappedIndex;

		if (n == 0)	// no swap, no need to compare right-to-left back
			return;

		for (int j = n - 1; j >= m; --j) {
			if (comp(*(first + j), *(first + j - 1))) {
				swap(first + j, first + j - 1);
				lastLeftSwappedIndex = j;
			}
		}
		m = lastLeftSwappedIndex;
	}

#else	// just left-to-right bubble sort
	// see also <https://en.wikipedia.org/wiki/Bubble_sort#Optimizing_bubble_sort>
	int n = last - first;	// unsorted length
	int lastSwappedIndex = 0;
	while (n > 1) {
		lastSwappedIndex = 0;
		for (int i = 1; i < n; ++i) {
			if (comp(*(first + i), *(first + i - 1))) {
				swap(first + i, first + i - 1);
				lastSwappedIndex = i;
			}
		}
		n = lastSwappedIndex;
	}
#endif // defined Cocktail_shaker_sort
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::CombSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Comb_sort>
	// the main idea is like Shellsort, but based on bubble sort (Shellsort is based on insertion sort)
	int n = last - first;	// unsorted length
	int gap = n;
	auto shrink = 1.3;	// gap shrink factor. 1.3 has been suggested
	bool sorted = false;
	while (sorted == false) {
		gap = int(gap / shrink);
		if (gap < 1) {
			gap = 1;
			sorted = true;
		}
		for (int i = 0; i + gap < n; ++i) {
			if (comp(*(first + i + gap), *(first + i))) {
				swap(first + i + gap, first + i);
				sorted = false;
			}
		}
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::CountingSort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::BucketSort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::RadixSort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
inline void sortingMethods<RandomIt, Compare>::swap(RandomIt it_1, RandomIt it_2)
{
	// swap content of two objects that are pointed by iterator (pointer) it_1 & it_2 respectively
	auto tmp{ std::move(*it_1) };
	*it_1 = std::move(*it_2);
	*it_2 = std::move(tmp);

	// or we should change the two nodes' links when swapping data is expensive
	// changing links operations...
}

}

#endif // !MYSORT_H
