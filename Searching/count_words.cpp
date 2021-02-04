/*
 *  Base file to be included.
 *  You can #define HASHMAP/TSTMAP/BSTMAP/STDMAP or leave
 *  it to dedault option which is std::unordered_map.
 */

#if defined(HASHMAP)
    #include "HashMap/HashMap.h"
#elif defined(BSTMAP)
    #include "TreeMap/BstMap.h"
#elif defined(TSTMAP)
    #include "TreeMap/TST.h"
#elif defined(STDMAP)
    #include <map>
#else
    #include <unordered_map>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // std::transform, std::partial_sort
#include <cctype>    // ::tolower
#include <ctime>

#define print_pair(it)      \
        std::cout << (*it)->first << " :  " << (*it)->second << '\n';
        //std::printf("  %-10s%10zu\n", (*it)->first.c_str(), (*it)->second);

#define TST_print_pair(it)  \
        std::cout << (*it).key() << " :  " << (*it).val() << '\n';
        //std::printf("  %-10s%10zu\n", (*it).key().c_str(), (*it).val());

#define print_range(begin, end)                      \
        for (auto it = begin; it != end; ++it)       \
            print_pair(it);

#define TST_print_range(begin, end)                  \
        for (auto it = begin; it != end; ++it)       \
            TST_print_pair(it);

#define print_k_times(k, it)                         \
        for (auto i = 0; i < k; ++i, ++it)           \
            print_pair(it);

#define TST_print_k_times(k, it)                     \
        for (auto i = 0; i < k; ++i, ++it)           \
            TST_print_pair(it);


// by Scott Meyers
template<typename MapIt>
void show_most_common_words(MapIt begin, MapIt end, size_t k)
{
    std::vector<MapIt> wordIters;
    wordIters.reserve(std::distance(begin, end)); // maybe we should pass its size() also
    for (auto i = begin; i != end; ++i)
        wordIters.push_back(i);

    auto sortedRangeEnd = wordIters.begin() + k;

#if defined(TSTMAP)
    std::partial_sort(wordIters.begin(), sortedRangeEnd, wordIters.end(),
        [](MapIt it1, MapIt it2) { return it1.val() > it2.val(); });

    if (k <= 10) { // print all if no more then 10 items
        TST_print_range(wordIters.cbegin(), sortedRangeEnd);
    }
    else { // print first 5 and last 5 items
        auto it = wordIters.cbegin();
        TST_print_k_times(5, it);
        std::cout << "...\n";
        it = sortedRangeEnd - 5;
        TST_print_k_times(5, it);
    }
#else
    std::partial_sort(wordIters.begin(), sortedRangeEnd, wordIters.end(),
        [](MapIt it1, MapIt it2) { return it1->second > it2->second; });

    if (k <= 10) { // print all if no more then 10 items
        print_range(wordIters.cbegin(), sortedRangeEnd);
    }
    else { // print first 5 and last 5 items
        auto it = wordIters.cbegin();
        print_k_times(5, it);
        std::cout << "...\n";
        it = sortedRangeEnd - 5;
        print_k_times(5, it);
    }
#endif
}

// CLI: ./count_words leipzig1M.txt 8 10000
// test data file: https://algs4.cs.princeton.edu/31elementary/leipzig1M.txt
int main(int argc, char* argv[])
{
    using namespace std;
    
    try {
        auto t0 = clock();
        std::ios_base::sync_with_stdio(false);
        
#if defined(HASHMAP)
        mySymbolTable::HashMap<string, size_t> mp{};
#elif defined(BSTMAP)
        mySymbolTable::BstMap<string, size_t> mp{};
#elif defined(TSTMAP)
        mySymbolTable::TST<size_t> mp{};
#elif defined(STDMAP)
        std::map<string, size_t> mp{};
#else
        std::unordered_map<string, size_t> mp{};
#endif
        
        size_t n{};  // pick out words whose length are >= n
        size_t k{};  // show top k most common words
        if (argc < 2) { cerr << "Too few arguments" << endl; return 1; }
        else if (argc == 2) { n = 1; k = 10; }
        else if (argc == 3) { n = std::stoi(argv[2]); k = 10; }
        else { n = std::stoi(argv[2]); k = std::stoi(argv[3]); }
        string filename{ argv[1] };
        ifstream ifs{ filename };
        if (!ifs.is_open()) { cerr << "Error opening file " << filename << endl; return 2; }

        for (string word; ifs >> word;) {
            if (word.length() < n) continue; // dump short word 
            transform(word.begin(), word.end(), word.begin(), ::tolower); // note that tolower is of global namespace
            ++mp[word];
        }
        auto t1 = clock();
        if (k > mp.size()) k = mp.size();
        show_most_common_words(mp.begin(), mp.end(), k);
        auto t2 = clock();

        auto build_time = (t1 - t0) / (double)CLOCKS_PER_SEC;
        auto sort_time  = (t2 - t1) / (double)CLOCKS_PER_SEC;
        auto total_time = (t2 - t0) / (double)CLOCKS_PER_SEC;

        cout << "\nbuild time: " << build_time << "s\n"
            << "sort time:  " << sort_time << "s\n"
            << "total: used " << total_time << "s to find the top " << k
            << " most common words (words length >=" << n << ")\n";
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
