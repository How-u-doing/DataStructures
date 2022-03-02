
// base file

#if defined(USE_HOARE)
  // default quicksort partition scheme
  #define __sort__    quicksort
  #define __method__  "Hoare"
#elif defined(USE_LOMUTO)
  #define LOMUTO_PARTITION
  #define __sort__    quicksort
  #define __method__  "Lomuto"
#elif defined(USE_Q3W)
  #define QUICK3WAY_PARTITION
  #define __sort__    quicksort
  #define __method__  "Quick3Way"
#elif defined(USE_F3W)
  #define FAST3WAY_PARTITION
  #define __sort__    quicksort
  #define __method__  "Fast3Way"
#elif defined(USE_MSD)
  #define __sort__    radix_sort
  #define __method__  "RadixSort"
#elif defined(USE_Q3S)
  #define __sort__    str_qsort
  #define __method__  "Quick3String"
#elif defined(USE_HEAPSORT)
  #define __sort__    heapsort
  #define __method__  "Heapsort"
#elif defined(USE_MERGE_SORT)
  #define __sort__    merge_sort
  #define __method__  "MergeSort"
#else
  #define __method__  "std::sort"
#endif

#if defined(__sort__)
#include "mySort.h"
#else
#include <algorithm> // std::sort
#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

// run ./string_sort < leipzig1M.txt
int main()
{
    vector<string> arr;
    string s;
    while ( cin >> s ) {
        arr.emplace_back(s);
    }

    auto t0 = clock();

#if defined(USE_HOARE) || defined(USE_LOMUTO) || defined(USE_Q3W) || \
    defined(USE_F3W) || defined(USE_MSD) || defined(USE_Q3S) || \
    defined(USE_MERGE_SORT) || defined(USE_HEAPSORT)
    namespace mysa = mySortingAlgo;
    mysa::__sort__(arr.begin(), arr.end());
#else
    std::sort(arr.begin(), arr.end());
#endif
    auto t1 = clock();

    auto time = (t1-t0) / (double) CLOCKS_PER_SEC;

    //for (const auto& x : arr)
    //    cout << x << '\n';
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i-1] > arr[i]) {
            cerr << "\033[0;31m  unsorted!\033[0m" << endl;
            return -1;
        }
    }

    // write results to a file
    ofstream("results.txt", std::ios_base::app) // rvalue overload
        << "| " << __method__ << " | " << time << "s |\n";

    cout << "sort time: " << time << "s\n";

    return 0;
}
