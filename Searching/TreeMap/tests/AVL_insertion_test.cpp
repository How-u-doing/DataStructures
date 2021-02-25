#include "../AvlMap.h"
#include <map>
#include <random>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>

const int N = 10;
const int Max = 1'000'000;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 1'000'000);

double check_insert()
{
    //std::map<int, std::string> map;
    mySymbolTable::AvlMap<int, std::string> map;
    const auto t0 = clock();
    for (int i = 0; i < Max; ++i) {
        const int x = distrib(gen);
        map.insert({ x, "A somewhat long string for test" });
        //map.insert({ x, "A short one" });
    }
    const auto t1 = clock();
    return (t1 - t0) / (double)CLOCKS_PER_SEC;
}

double check_insert_with_hint()
{
    //std::map<int, std::string> map;
    mySymbolTable::AvlMap<int, std::string> map;
    const auto t0 = clock();
    for (int i = 0; i < Max; ++i) {
        const int x = distrib(gen);
        const auto iter = map.lower_bound(x);
        if (iter == map.end() || iter->first != x) {
            map.insert(iter, { x, "A somewhat long string for test" });
            //map.insert(iter, { x, "A short one" });
        }
    }
    const auto t1 = clock();
    return (t1 - t0) / (double)CLOCKS_PER_SEC;
}

double check_subscript()
{
    //std::map<int, std::string> map;
    mySymbolTable::AvlMap<int, std::string> map;
    const auto t0 = clock();
    for (int i = 0; i < Max; ++i) {
        const int x = distrib(gen);
        map[x] = "A somewhat long string for test";
        //map[x] = "A short one";
    }
    const auto t1 = clock();
    return (t1 - t0) / (double)CLOCKS_PER_SEC;
}

int main()
{
    // which insertion pattern which I choose?
    std::cout << "      tIns" << "      tHin" << "      tSub\n";
    for (int i = 1; i <= N; ++i) {
        double tIns = check_insert();
        double tHin = check_insert_with_hint();
        double tSub = check_subscript();
        std::cout << std::right
            << std::setw(10) << tIns
            << std::setw(10) << tHin
            << std::setw(10) << tSub << '\n';
    }
    // done
    return 0;
}