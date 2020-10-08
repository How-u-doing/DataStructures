/**
 *  see this file at following link for the latest version
 *  https://github.com/How-u-doing/DataStructures/blob/master/Sorting/compare2c/mySort2.h
 */
#ifndef MYSORT2_H
#define MYSORT2_H
#include <utility>	  // std::move

namespace mySortingAlgo2{

const int ISORT_MAX = 32;	// maximum size for insertion sort

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
		for (auto j = i; j > 0 && comp(first + j, first + j - 1); --j)
			iter_swap(first + j, first + j - 1);
	}
}

// make high = median of {low, mid, high}
template<typename RandomIt, typename Compare>
inline void mid3(RandomIt low, RandomIt high, Compare comp)
{
	auto mid = low + ((high - low) >> 1);
	if (comp(mid, low))  iter_swap(low, mid);
	if (comp(high, low)) iter_swap(low, high);
	if (comp(mid, high)) iter_swap(mid, high);
}


template<typename RandomIt, typename Compare>
RandomIt Hoare_partition(RandomIt low, RandomIt high, Compare comp)
{
	// see also <https://en.wikipedia.org/wiki/quicksort#Hoare_partition_scheme>
	mid3(low, high, comp);
	auto pivot = low + ((high - low) >> 1);
	auto i = low, j = high;
	while (true) {
		while (comp(i, pivot)) ++i;
		while (comp(pivot, j)) --j;
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

	auto pivot = Hoare_partition(first, last - 1, comp);
	quicksort(first, pivot + 1, comp);	// sort a[frist..pivot]
	quicksort(pivot + 1, last, comp);	// sort a[pivot+1..last-1]
}

}// namespace mySortingAlgo2

#endif // !MYSORT2_H
