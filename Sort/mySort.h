#pragma once
#ifndef MYSORT_H
#define MYSOER_H

namespace mySortingAlgo{

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

	static inline void swap_content(RandomIt it_1, RandomIt it_2);
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
//#define DIRECT_INSERTION_SORT
	// see also <https://en.wikipedia.org/wiki/Insertion_sort>
#if defined DIRECT_INSERTION_SORT
	// direct insertion sort
	int len = last - first;
	for (int i = 1; i < len; ++i) {
		for (int j = i; j > 0 && comp(*(first + j), *(first + j - 1)); --j) {
			swap_content(first + j, first + j - 1);
		}
}
#else	// When we have massive data items, we can save some comparison time by using binary search.

	for (auto it = first + 1; it != last; ++it) {
		auto pos = binary_search(first, it, it, comp);
		if (pos == it)
			continue;

		// insert *it before position 'pos', move by block
		auto tmp = *it;
		for (auto iter = it; iter != pos; --iter)
			*iter = *(iter - 1);
		*pos = tmp;
	}

#endif // defined DIRECT_INSERTION_SORT
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
			while (!comp(*M, *it) && !comp(*it, *M)) // *M == *it
				++it;

			return it;
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
			swap_content(jM, i);
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::MergeSort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Heapsort(RandomIt first, RandomIt last, Compare comp)
{
}

// quicksort elements in [first, last-1]
template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Quicksort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Quicksort>
	if (last - first > 1) {
		auto pivot = partition(first, last - 1, comp);
// define your partition scheme
//#define Lomuto_partition
//#define Hoare_partition
#define Hoare_partition
#if defined Lomuto_partition
		Quicksort(first, pivot, comp);		// [first, pivot-1]
		Quicksort(pivot + 1, last, comp);	// [pivot+1, last-1]
#elif defined Hoare_partition
		Quicksort(first, pivot + 1, comp);	// [frist, pivot]
		Quicksort(pivot + 1, last, comp);	// [pivot+1, last-1]
#endif // defined Lomuto_partition
	}
}

// partition ranging [left, right]
template<typename RandomIt, typename Compare>
RandomIt sortingMethods<RandomIt, Compare>::partition(RandomIt low, RandomIt high, Compare comp)
{
#if defined Lomuto_partition
	// see 'CLRS-3e' p171-172 (illustration, Fig7.1)
	// we can also choose a random iterator in [lo, hi] as pivot (i.e. k=RANDOM(lo,hi)), then swap(k, hi) if k!=hi.
	// see also <https://en.wikipedia.org/wiki/Quicksort#Lomuto_partition_scheme>
	auto pivot = *high;	 // choose the right most element as pivot
	auto i = low;	// [low...i-1] < pivot, [i...high] >= pivot. (operator<, ascending)
	for (auto j = low; j != high; ++j) {
		if (comp(*j, pivot)) {
			if (i != j)
				swap_content(i, j);
			++i;
		}
	}
	swap_content(i, high);
	return i;

#elif defined Hoare_partition
	// see also <https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme>
	auto pivot = *(low + ((high - low) >> 1));	// pivot=*(low+(high-low)/2);
	auto i = low, j = high;	// [low...i-1] < pivot, [i...high] >= pivot. (operator<, ascending)
	while (true) {
		while (comp(*i, pivot)) {
			++i;
		}
		while (comp(pivot, *j)) {
			--j;
		}
		if (i >= j) {
			return j;
		}
		swap_content(i++, j--);
	}


#endif // defined Lomuto_partition

	

}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Shellsort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::BubbleSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Bubble_sort#Optimizing_bubble_sort>
	int n = last - first;	// unsorted length
	int lastSwappedIndex = 0;
	while (n > 1) {
		lastSwappedIndex = 0;
		for (int i = 1; i < n; ++i) {
			if (comp(*(first + i), *(first + i - 1))) {
				swap_content(first + i, first + i - 1);
				lastSwappedIndex = i;
			}
		}
		n = lastSwappedIndex;
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::CombSort(RandomIt first, RandomIt last, Compare comp)
{
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
inline void sortingMethods<RandomIt, Compare>::swap_content(RandomIt it_1, RandomIt it_2)
{
	auto tmp = *it_1;
	*it_1 = *it_2;
	*it_2 = tmp;
}

}

#endif // !MYSORT_H
