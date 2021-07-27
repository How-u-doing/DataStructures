#include "../RbMap.h"
#include <random>
#include <string>
#include <iostream>

const int Max = 3'000;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 1'000);

int main()
{
    mySymbolTable::RbMap<int, std::string> map;

    for (int i = 0; i < Max; ++i) {
        const int x = distrib(gen);
        map[x] = "Some data";        
        if (!map.is_rb_tree()) {
            std::cout << "Not a red-black tree\n";
            return -1;
        }
    }
    std::cout << "randomly inserted " << map.size() << " entries\n";
    auto count0 = map.size();

    for (int i = 0; i < 1'000; ++i) {
        const int x = distrib(gen);
        map.erase(x);
        if (!map.is_rb_tree()) {
            std::cout << "Not a red-black tree\n";
            return -1;
        }
    }
    std::cout << "randomly erased " << count0 - map.size() << " entries\n";

    // if we see these two messages means the insertion and deletion
    // subroutines have no problems, and the tree remains balanced.
    std::cout << "Excellent! All tests passed! The red-black tree remained balanced:\n";
    map.print();
    return 0;
}
