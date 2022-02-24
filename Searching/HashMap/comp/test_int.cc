
// define your method here
//#define USE_HASHMAP

#if defined(USE_HASHMAP)
#include "../HashMap.h"
#elif defined(USE_HASHMAP2)
#include "../alternative/HashMap2.h"
#else
#include <unordered_map>
#endif

#include <iostream>
#include <ctime>

int main()
{
#if   defined(USE_HASHMAP)
    mySymbolTable::HashMap
#elif defined(USE_HASHMAP2)
    mySymbolTable::alternative::HashMap
#else
    std::unordered_map
#endif
        <int, int> mp{};

    constexpr int N = 1'000'000;
    int k = 0;

    auto t0 = std::clock();
    for (int i = 0; i < N; ++i) {
        mp[N-i] = i;
        //std::cout << "\nAfter inserting " << i << '\n';
        //mp.print();
    }
    auto t1 = std::clock();

    for(auto it=mp.begin(); it!=mp.end(); ++it)
        ++k;
    auto t2= std::clock();

    auto build_time_elapsed = (t1 - t0) / (double)1'000;
    auto iteration_time_elapsed = (t2 - t1) / (double)1'000;

    std::cout << "Building " << mp.size() << " items used " << build_time_elapsed << "ms\n"
              << "Iterating through " << k << " items used " << iteration_time_elapsed << "ms\n";

    return 0;
}
