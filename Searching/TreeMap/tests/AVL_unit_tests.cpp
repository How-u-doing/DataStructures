/*
 *   Tests from cppreference.com
 */
#include "../AvlMap.h"
//#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#define __myns__            myst
#define __my_map_type__     AvlMap

namespace myst = mySymbolTable;
using namespace std::literals;

template<typename It>
void printInsertionStatus(It it, bool success)
{
    std::cout << "Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

void test_insert()
{
    __myns__::__my_map_type__<std::string, float> karasunoPlayerHeights;
    // Overload 3: insert from rvalue reference
    const auto [it_hinata, success] = karasunoPlayerHeights.insert({"Hinata"s, 162.8});
    printInsertionStatus(it_hinata, success);

    {
        // Overload 1: insert from lvalue reference
        const auto [it, success2] = karasunoPlayerHeights.insert(*it_hinata);
        printInsertionStatus(it, success2);
    }
    {
        // Overload 2: insert via forwarding to emplace
        const auto [it, success] = karasunoPlayerHeights.insert(std::pair{"Kageyama", 180.6});
        printInsertionStatus(it, success);
    }

    {
        // Overload 6: insert from rvalue reference with positional hint
        const std::size_t n = std::size(karasunoPlayerHeights);
        const auto it = karasunoPlayerHeights.insert(it_hinata, {"Azumane"s, 184.7});
        printInsertionStatus(it, std::size(karasunoPlayerHeights) != n);
    }
    {
        // Overload 4: insert from lvalue reference with positional hint
        const std::size_t n = std::size(karasunoPlayerHeights);
        const auto it = karasunoPlayerHeights.insert(it_hinata, *it_hinata);
        printInsertionStatus(it, std::size(karasunoPlayerHeights) != n);
    }
    {
        // Overload 5: insert via forwarding to emplace with positional hint
        const std::size_t n = std::size(karasunoPlayerHeights);
        const auto it = karasunoPlayerHeights.insert(it_hinata, std::pair{"Tsukishima", 188.3});
        printInsertionStatus(it, std::size(karasunoPlayerHeights) != n);
    }

    // Overload 7: insert from iterator range
    __myns__::__my_map_type__<std::string, float> playerHeights;
    playerHeights.insert(std::begin(karasunoPlayerHeights), std::end(karasunoPlayerHeights));

    // Overload 8: insert from initializer_list
    playerHeights.insert({{"Kozume"s, 169.2}, {"Kuroo", 187.7}});


    // Print resulting map
    std::cout << std::left << '\n';
    for (const auto& [name, height] : playerHeights)
        std::cout << std::setw(10) << name << " | " << height << "cm\n";
}

auto print_node = [](const auto &node) {
    std::cout << "[" << node.first << "] = " << node.second << '\n';
};

auto print_result = [](auto const &pair) {
    std::cout << (pair.second ? "inserted: " : "assigned: ");
    print_node(*pair.first);
};

void test_insert_or_assign()
{
    __myns__::__my_map_type__<std::string, std::string> myMap;
    print_result( myMap.insert_or_assign("a", "apple"     ) );
    print_result( myMap.insert_or_assign("b", "banana"    ) );
    print_result( myMap.insert_or_assign("c", "cherry"    ) );
    print_result( myMap.insert_or_assign("c", "clementine") );

    for (const auto &node : myMap) { print_node(node); }
}

void test_emplace()
{
    __myns__::__my_map_type__<std::string, std::string> m;
    // uses pair's move constructor
    m.emplace(std::make_pair(std::string("a"), std::string("a")));

    // uses pair's converting move constructor
    m.emplace(std::make_pair("b", "abcd"));

    // uses pair's template constructor
    m.emplace("d", "ddd");

    // uses pair's piecewise constructor
    m.emplace(std::piecewise_construct,
              std::forward_as_tuple("c"),
              std::forward_as_tuple(10, 'c'));
    // as of C++17, m.try_emplace("c", 10, 'c'); can be used

    for (const auto &p : m) {
        std::cout << p.first << " => " << p.second << '\n';
    }
}

const int nof_operations = 100500;

int map_emplace() {
    __myns__::__my_map_type__<int, char> map;
    for(int i = 0; i < nof_operations; ++i) {
        map.emplace(i, 'a');
    }
    return map.size();
}

int map_emplace_hint() {
    __myns__::__my_map_type__<int, char> map;
    auto it = map.begin();
    for(int i = 0; i < nof_operations; ++i) {
        map.emplace_hint(it, i, 'b');
        it = map.end();
    }
    return map.size();
}

int map_emplace_hint_wrong() {
    __myns__::__my_map_type__<int, char> map;
    auto it = map.begin();
    for(int i = nof_operations; i > 0; --i) {
        map.emplace_hint(it, i, 'c');
        it = map.end();
    }
    return map.size();
}

int map_emplace_hint_corrected() {
    __myns__::__my_map_type__<int, char> map;
    auto it = map.begin();
    for(int i = nof_operations; i > 0; --i) {
        map.emplace_hint(it, i, 'd');
        it = map.begin();
    }
    return map.size();
}

int map_emplace_hint_closest() {
    __myns__::__my_map_type__<int, char> map;
    auto it = map.begin();
    for(int i = 0; i < nof_operations; ++i) {
        it = map.emplace_hint(it, i, 'e');
    }
    return map.size();
}

void timeit(std::function<int()> map_test, std::string what = "") {
    auto start = std::chrono::system_clock::now();
    int mapsize = map_test();
    auto stop = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> time = stop - start;
    if (what.size() > 0 && mapsize > 0) {
        std::cout << std::fixed << std::setprecision(2) << std::setw(5)
            << time.count() << "  ms for " << what << '\n';
    }
}

void test_emplace_hint() {
    timeit(map_emplace); // stack warmup
    timeit(map_emplace, "plain emplace");
    timeit(map_emplace_hint, "emplace with correct hint");
    timeit(map_emplace_hint_wrong, "emplace with wrong hint");
    timeit(map_emplace_hint_corrected, "corrected emplace");
    timeit(map_emplace_hint_closest, "emplace using returned iterator");
}

void test_try_emplace()
{
    using namespace std::literals;
    __myns__::__my_map_type__<std::string, std::string> m;
    print_result( m.try_emplace("a", "a"s) );
    print_result( m.try_emplace("b", "abcd") );
    print_result( m.try_emplace("c", 10, 'c') );
    print_result( m.try_emplace("c", "Won't be inserted") );

    for (const auto &p : m) { print_node(p); }
}

void test_erase()
{
    __myns__::__my_map_type__<int, std::string> c = {
        {1, "one" }, {2, "two" }, {3, "three"},
        {4, "four"}, {5, "five"}, {6, "six"  }
    };

    // erase all odd numbers from c
    for(auto it = c.begin(); it != c.end(); ) {
        if(it->first % 2 != 0)
            it = c.erase(it);
        else
            ++it;
    }

    for(auto& p : c) {
        std::cout << p.second << ' ';
    }
    std::cout << '\n';
}

// print out a std::pair
template <class Os, class U, class V>
Os& operator<<(Os& os, const std::pair<U, V>& p) {
    return os << p.first << ":" << p.second;
}

// print out a container
template <class Os, class Co>
Os& operator<<(Os& os, const Co& co) {
    os << "{";
    for (auto const& i : co) { os << ' ' << i; }
    return os << " }\n";
}

void test_swap()
{
    __myns__::__my_map_type__<std::string, std::string>
        m1 { {"γ", "gamma"}, {"β", "beta"}, {"α", "alpha"}, {"γ", "gamma"}, },
        m2 { {"ε", "epsilon"}, {"δ", "delta"}, {"ε", "epsilon"} };

    const auto& ref = *(m1.begin());
    const auto iter = std::next(m1.cbegin());

    std::cout << "──────── before swap ────────\n"
              << "m1: " << m1 << "m2: " << m2 << "ref: " << ref
              << "\niter: " << *iter << '\n';

    m1.swap(m2);

    std::cout << "──────── after swap ────────\n"
              << "m1: " << m1 << "m2: " << m2 << "ref: " << ref
              << "\niter: " << *iter << '\n';

    // Note that every iterator referring to an element in one container before
    // the swap refers to the same element in the other container after the swap.
    // Same is true for references.
}

auto print = [](auto const comment, auto const& map) {
    std::cout << comment << "{";
    for (const auto &pair : map) {
        std::cout << "{" << pair.first << ": " << pair.second << "}";
    }
    std::cout << "}\n";
};

void test_subsrcipt_op()
{
    __myns__::__my_map_type__<char, int> letter_counts {{'a', 27}, {'b', 3}, {'c', 1}};

    print("letter_counts initially contains: ", letter_counts);

    letter_counts['b'] = 42;  // updates an existing value
    letter_counts['x'] = 9;  // inserts a new value

    print("after modifications it contains: ", letter_counts);

    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    __myns__::__my_map_type__<std::string, int> word_map;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"}) {
        ++word_map[w];
    }
    word_map["that"]; // just inserts the pair {"that", 0}

    for (const auto &[word, count] : word_map) {
        std::cout << count << " occurrences of word '" << word << "'\n";
    }
}

template<typename Map>
void print_map(Map& m)
{
   std::cout << '{';
   for(auto& p: m)
        std::cout << p.first << ':' << p.second << ' ';
   std::cout << "}\n";
}

struct Point { double x, y; };
struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const {
        return lhs.x < rhs.x; // NB. intentionally ignores y
    }
};

void test_ctor()
{
    // (1) Default constructor
    __myns__::__my_map_type__<std::string, int> map1;
    map1["something"] = 69;
    map1["anything"] = 199;
    map1["that thing"] = 50;
    std::cout << "map1 = "; print_map(map1);

    // (2) Range constructor
    __myns__::__my_map_type__<std::string, int> iter(map1.find("anything"), map1.end());
    std::cout << "\niter = "; print_map(iter);
    std::cout << "map1 = "; print_map(map1);

    // (3) Copy constructor
    __myns__::__my_map_type__<std::string, int> copied(map1);
    std::cout << "\ncopied = "; print_map(copied);
    std::cout << "map1 = "; print_map(map1);

    // (4) Move constructor
    __myns__::__my_map_type__<std::string, int> moved(std::move(map1));
    std::cout << "\nmoved = "; print_map(moved);
    std::cout << "map1 = "; print_map(map1);

    // (5) Initializer list constructor
    const __myns__::__my_map_type__<std::string, int> init {
        {"this", 100},
        {"can", 100},
        {"be", 100},
        {"const", 100},
    };
    std::cout << "\ninit = "; print_map(init);


    // Custom Key class option 1:
    // Use a comparison struct
    __myns__::__my_map_type__<Point, double, PointCmp> mag = {
        { {5, -12}, 13 },
        { {3, 4},   5 },
        { {-8, -15}, 17 }
    };

    for(auto p : mag)
        std::cout << "The magnitude of (" << p.first.x
            << ", " << p.first.y << ") is "
            << p.second << '\n';

    // Custom Key class option 2:
    // Use a comparison lambda
    // This lambda sorts points according to their magnitudes, where note that
    //  these magnitudes are taken from the local variable mag
    auto cmpLambda = [&mag](const Point &lhs, const Point &rhs) { return mag[lhs] < mag[rhs]; };
    //You could also use a lambda that is not dependent on local variables, like this:
    //auto cmpLambda = [](const Point &lhs, const Point &rhs) { return lhs.y < rhs.y; };
    __myns__::__my_map_type__<Point, double, decltype(cmpLambda)> magy(cmpLambda);

    //Various ways of inserting elements:
    magy.insert(std::pair<Point, double>({5, -12}, 13));
    magy.insert({ {3, 4}, 5});
    magy.insert({Point{-8.0, -15.0}, 17});

    std::cout << '\n';
    for(auto p : magy)
        std::cout << "The magnitude of (" << p.first.x
            << ", " << p.first.y << ") is "
            << p.second << '\n';
}

int main()
{
    test_insert();
    test_insert_or_assign();
    test_emplace();
    test_emplace_hint();
    test_try_emplace();
    test_erase();
    test_swap();
    test_subsrcipt_op();
    test_ctor();
    return 0;
}
