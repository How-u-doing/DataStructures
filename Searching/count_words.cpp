#if defined(USE_STDMAP)
#   include <map>
#elif defined(USE_AVL)
#   include "TreeMap/AvlMap.h"
#elif defined(USE_BST)
#   include "TreeMap/BstMap.h"
#elif defined(USE_RBT)
#   include "TreeMap/RbMap.h"
#elif defined(USE_TST)
#   include "TreeMap/TST.h"
#elif defined(USE_MYHT)
#   include "HashMap/HashMap.h"
#elif defined(USE_MYHT2)
#   include "HashMap/alternative/HashMap2.h"
#elif defined(USE_SKIPLIST)
#   include "Randomized/SkiplistMap.h"
#else
#   include <unordered_map>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm> // std::transform, std::partial_sort
#include <cctype>    // ::tolower
#include <ctime>

#define print_pair(it)      \
        std::cout << (*it)->first << " : " << (*it)->second << '\n';
        //std::printf("  %-10s%10zu\n", (*it)->first.c_str(), (*it)->second);

#define TST_print_pair(it)  \
        std::cout << (*it).key() << " : " << (*it).val() << '\n';
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
void show_top_k_most_common_words(MapIt begin, MapIt end, size_t k)
{
    std::vector<MapIt> wordIters;
    // maybe we should also pass its size()
    // iterating through the symbol table acctually takes most of the time
    size_t n = std::distance(begin, end);
    wordIters.reserve(n);
    for (auto i = begin; i != end; ++i)
        wordIters.push_back(i);
    auto sortedRangeEnd = wordIters.begin() + k;

#if defined(USE_TST)
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

using namespace std;

// run: ./count_words leipzig1M.txt leipzig100K.txt 4 10000
// corpus file: https://algs4.cs.princeton.edu/31elementary/leipzig1M.txt
// query  file: https://algs4.cs.princeton.edu/31elementary/leipzig100K.txt
int main(int argc, char* argv[])
{
    try {
        std::ios_base::sync_with_stdio(false);
        const char* method{};
#if defined(USE_MYHT)
        mySymbolTable::HashMap<string, size_t> mp{};
        method = "myst::Hashtable";
#elif defined(USE_MYHT2)
        mySymbolTable::alternative::HashMap<string, size_t> mp{};
        method = "myst::Hashtable2";
#elif defined(USE_BST)
        mySymbolTable::BstMap<string, size_t> mp{};
        method = "myst::BST";
#elif defined(USE_AVL)
        mySymbolTable::AvlMap<string, size_t> mp{};
        method = "myst::AVL";
#elif defined(USE_RBT)
        mySymbolTable::RbMap<string, size_t> mp{};
        method = "myst::RBtree";
#elif defined(USE_TST)
        mySymbolTable::TST<size_t> mp{};
        method = "myst::TST";
#elif defined(USE_STDMAP)
        std::map<string, size_t> mp{};
        method = "std::map";
#elif defined(USE_SKIPLIST)
        mySymbolTable::SkiplistMap<string, size_t> mp{};
        method = "myst::SkipList";
#else
        std::unordered_map<string, size_t> mp{};
        method = "std::unordered_map";
#endif

        size_t n{};  // word length >= n
        size_t k{};  // show top k most common words
        if (argc < 3) {
            cout << "Usage: " << argv[0] << " CORPUS_FILE QUERY_FILE "
                 << "[WORD_LENGTH>=1] [SHOW_TOP_K=10]\n";
            return 0;
        }
        if (argc == 3) { n = 1; k = 10; }
        else if (argc == 4) { n = std::stoi(argv[3]); k = 10; }
        else { n = std::stoi(argv[3]); k = std::stoi(argv[4]); }

        string corpus_file{ argv[1] };
        ifstream ifs{ corpus_file };
        if (!ifs.is_open()) {
            cerr << "cannot open corpus file " << corpus_file << endl;
            return EXIT_FAILURE;
        }
        string query_file{ argv[2] };
        ifstream ifs2{ query_file };
        if (!ifs2.is_open()) {
            cerr << "cannot open query file " << query_file << endl;
            return EXIT_FAILURE;
        }

        std::random_device rd;
        std::mt19937 g(rd());
        vector<string> queries;
        for (string query; ifs2 >> query; ) {
            queries.push_back(query);
        }
        // let's do random queries
        //std::shuffle(queries.begin(), queries.end(), g);

        auto t0 = clock();
        for (string word; ifs >> word; ) {
            if (word.length() < n) continue; // discard short words
            // note that `tolower` is of global namespace
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            ++mp[word];
        }
        auto t1 = clock();
        if (k > mp.size()) k = mp.size();
        show_top_k_most_common_words(mp.begin(), mp.end(), k);
        auto t2 = clock();
        size_t max_freq = 0;
        size_t cur_freq = 0;
        for (const string& query : queries) {
            auto it = mp.find(query);
            // do something...
            if (it != mp.end()) {
#if !defined(USE_TST)
                cur_freq = it->second;
#else
                cur_freq = it.val();
#endif
                if (max_freq < cur_freq)
                    max_freq = cur_freq;
            }
        }
        auto t3 = clock();
        size_t sz = 0;
        for (auto it = mp.begin(); it != mp.end(); ++it)
            ++sz; // prevent the compiler from optimizing away the empty loop
        auto t4 = clock();
        if (sz != mp.size()) {
            cerr << "detected an iterating bug" << endl;
            return EXIT_FAILURE;
        }

        auto build_time = (t1 - t0) / (double) CLOCKS_PER_SEC * 1000;
        auto sort_time  = (t2 - t1) / (double) CLOCKS_PER_SEC * 1000;
        auto find_time  = (t2 - t0) / (double) CLOCKS_PER_SEC * 1000;
        auto query_time = (t3 - t2) / (double) CLOCKS_PER_SEC * 1000;
        auto iter_time  = (t4 - t3) / (double) CLOCKS_PER_SEC * 1000;

        // if we don't use max_freq, the query loop will be optimized away
        cout << "max freq in corpus being queried is " << max_freq << "\n\n";
        cout << "build time: " << build_time << " ms\n"
             << "sort time:  " << sort_time << " ms\n"
             << "total: used " << find_time << " ms to find the top " << k
             << " most common words (word length >= " << n << ")\n"
             << "query time: " << query_time << " ms\n"
             << "iter time:  " << iter_time << " ms\n";

        [[maybe_unused]] int height = 0;
#if defined(USE_BST) || defined(USE_AVL) || defined(USE_RBT) || defined(USE_TST)
        cout << "\ntree size: " << mp.size()
             << "\ntree height: " << (height = mp.height()) << '\n';
#elif defined(USE_SKIPLIST)
        cout << "\nskip list size: " << mp.size()
             << "\nskip list height (max level): "
             << (height = mp.level()) << '\n';
#endif

        // write results to a file
        ofstream("results.txt", std::ios_base::app) // rvalue overload
            << "| " << method << " | " << build_time << " ms | "
            << sort_time << " ms | " << find_time << " ms | "
            << query_time << " ms | " << iter_time << " ms |"
#if defined(USE_BST) || defined(USE_AVL) || defined(USE_RBT) || \
    defined(USE_TST) || defined(USE_SKIPLIST)
            << ' ' << height << " |"
#endif
            << '\n';
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
