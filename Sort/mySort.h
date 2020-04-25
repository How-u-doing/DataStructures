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
	static void SelectionSort(RandomIt first, RandomIt last, Compare comp);
	static void MergeSort(RandomIt first, RandomIt last, Compare comp);
	static void Heapsort(RandomIt first, RandomIt last, Compare comp);
	static void Quicksort(RandomIt first, RandomIt last, Compare comp);
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

// This one has a drawback that cannot call lambda function
//template<class RandomIt, class U>
//void sort(RandomIt first, RandomIt last, bool (*comp)(const U& a, const U& b), Mode mode = Mode::Quicksort)
//{
//	// implementation...
//}
// Or like this using only one template argument via decltype
//template<class RandomIt>
//void sort(RandomIt first, RandomIt last, bool (*comp)(const decltype(*first)& a, const decltype(*first)& b), Mode mode = Mode::Quicksort)
//{
//	// implementation...
//}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::InsertionSort(RandomIt first, RandomIt last, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/Insertion_sort>
	// direct insertion sort
	/*int len = last - first;
	for (int i = 1; i < len; ++i) {
		for (int j = i; j > 0 && comp(*(first + j), *(first + j - 1)); --j) {
			swap_content(first + j, first + j - 1);
		}
	}*/

	//=========================================================================
	//=========================================================================

	// When we have massive data items, we can save some comparison time by
	// using binary search, see <https://en.wikipedia.org/wiki/Binary_search_algorithm>.
	// determine the sorting order: ascending or descending
	// of course, overloading operator< prior to comparison is needed
	int order{ 0 };
	if (last - first > 1) {
		if (*first < *(first + 1)) {
			if (comp(*first, *(first + 1)))
				order = 1;		// ascending
			else
				order = -1;		// descending
		}
		else {// *first >= *(first + 1)
			if (comp(*first, *(first + 1)))
				order = -1;		// descending
			else
				order = 1;		// ascending
		}
	}
	else
		return;

	decltype(first) L{ }, R{ }, M{ };
	// we can decrease the times of comparison of sorting
	// order to 1 with the cost of somewhat repetitive code
	if (order == 1) {// in ascending order
		for (auto it = first + 1; it != last; ++it) {
			L = first; R = it - 1;
			while (L <= R) {
				M = L + (R - L) / 2;	// how about M=(L+R)/2 ?
				if (*it < *M) {
					if (M == first)
						break;
					else
						R = M - 1;
				}
				else
					L = M + 1;
			}
			// insert *it right at position L, move by block
			auto tmp = *it;
			for (auto iter = it; iter != L; --iter)
				*iter = *(iter - 1);
			*L = tmp;
		}
	}
	else {// in descending order
		for (auto it = first + 1; it != last; ++it) {
			L = first; R = it - 1;
			while (L <= R) {
				M = L + (R - L) / 2;
				if (*M < *it) {
					if (M == first)
						break;
					else
						R = M - 1;
				}
				else
					L = M + 1;
			}
			// insert *it right at position L, move by block
			auto tmp = *it;
			for (auto iter = it; iter != L; --iter)
				*iter = *(iter - 1);
			*L = tmp;
		}
	}
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::SelectionSort(RandomIt first, RandomIt last, Compare comp)
{

}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::MergeSort(RandomIt first, RandomIt last, Compare comp)
{
}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Heapsort(RandomIt first, RandomIt last, Compare comp)
{

}

template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::Quicksort(RandomIt first, RandomIt last, Compare comp)
{
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
