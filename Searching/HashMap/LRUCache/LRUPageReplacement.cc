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

#define PAGE_CLEAN 0
#define PAGE_DIRTY 1

int main(int argc, char *argv[])
{
    ios::sync_with_stdio(false);
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <DELIMITER>\n";
        return EXIT_FAILURE;
    }
    size_t cache_size, pageno;
    cin >> cache_size;
    LRUCache<size_t, int> lru_cache(cache_size);

    string op;
    while ( true ) {
        cin >> op;
        if (op == "r") {
            cin >> pageno;
            auto iter = lru_cache.get(pageno);
            if (iter != lru_cache.end()) {
                int dirty = iter->second;
                if (dirty) cout << "[ \033[0;31m" << dirty << "\033[0m ]"; // dirty: red
                else       cout << "[ \033[0;32m" << dirty << "\033[0m ]"; // clean: green
                cout << ", ";
                print_cache(lru_cache.begin(), lru_cache.end());
            }
            else {
                cout << "Read miss in page " << pageno << '\n';
                // meanwhile bring in the missing page
                lru_cache.put(pageno, PAGE_CLEAN);
            }
        }
        else if (op == "w") {
            cin >> pageno;
            bool hit = lru_cache.put(pageno, PAGE_DIRTY);
            if (!hit)
                cout << "Write miss in page " << pageno << '\n';
        }
        else if (op == argv[1]) {
            break;
        }
        else {
            cerr << "Invalid operation" << endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
