#include "LRUCache.h"
#include <iostream>
#include <string>

using namespace std;

template<typename CacheIter>
void print_cache(CacheIter begin, CacheIter end) {
    cout << "Cache = [ ";
    for (CacheIter it = begin; it != end; ++it) {
        cout << '[' << it->first << ", " << it->second << "]";
        CacheIter tmp = it;
        if (++tmp != end) cout << ", ";
    }
    cout << " ]\n";
}

int main()
{
    try {
        ios::sync_with_stdio(false);

        int cache_size, nops, key, val;
        cin >> cache_size >> nops;
        LRUCache<int, int> lru_cache(cache_size);

        string op;
        for (int i = 0; i < nops; ++i) {
            cin >> op;
            if (op == "get") {
                cin >> key;
                auto iter = lru_cache.get(key);
                if (iter != lru_cache.end()) {
                    cout << "[ \033[0;32m" << iter->second << "\033[0m ]";
                    cout << ", ";
                    print_cache(lru_cache.begin(), lru_cache.end());
                }
                else cout << "Cache miss in reading key=" << key << '\n';
            }
            else if (op == "put") {
                cin >> key >> val;
                lru_cache.put(key, val);
            }
            else {
                cerr << "Invalid operation read" << endl;
                return EXIT_FAILURE;
            }
        }
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "Unknown error" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
