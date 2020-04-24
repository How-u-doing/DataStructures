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

	// Distribution sort
	CountingSort, BucketSort, RadixSort

};

template<typename RandomIt, typename Compare>
class sortingMethods {
public:
	static void BubbleSort(RandomIt first, RandomIt last, Compare comp);
	//... to be added others

	static inline void swap(RandomIt it_1, RandomIt it_2);
};

template<typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp, Mode mode = Mode::Quicksort)
{
	switch (mode)
	{
	case Mode::InsertionSort:
		break;
	case Mode::SelectionSort:
		break;
	case Mode::MergeSort:
		break;
	case Mode::Heapsort:
		break;
	case Mode::Quicksort:
		break;
	case Mode::Shellsort:
		break;
	case Mode::BubbleSort:
		sortingMethods<RandomIt, Compare>::BubbleSort(first, last, comp);
		break;
	case Mode::CombSort:
		break;
	case Mode::CountingSort:
		break;
	case Mode::BucketSort:
		break;
	case Mode::RadixSort:
		break;
	default:
		break;
	}
}

// This one has a drawback that cannot call lambda function
//template<class RandomIt, class U>
//void sort(RandomIt first, RandomIt last, bool (*comp)(const U& a, const U& b))
//{
//	// implementation...
//}


template<typename RandomIt, typename Compare>
void sortingMethods<RandomIt, Compare>::BubbleSort(RandomIt first, RandomIt last, Compare comp)
{
	int n = last - first;
	while (n >= 1) {
		int lastSwappedIndex = 0;
		for (int i = 0; i < n - 1; ++i) {
			if (comp(*(first + i + 1), *(first + i))) {
				swap(first + i, first + i + 1);
				lastSwappedIndex = i;
			}
		}
		n = lastSwappedIndex;
	}
}

template<typename RandomIt, typename Compare>
inline void sortingMethods<RandomIt, Compare>::swap(RandomIt it_1, RandomIt it_2)
{
	auto tmp = *it_1;
	*it_1 = *it_2;
	*it_2 = tmp;
}

}

#endif // !MYSORT_H
