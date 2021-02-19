#include "../SkiplistMap.h"
#include <string>
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

template<typename Map>
void print_map(std::string_view comment, const Map& m)
{
    std::cout << comment;
    for (auto i = m.begin(); i != m.end(); ++i) {
        std::cout << '{' << i->first << ", " << i->second << "} : "
            << m.level(i) << '\n';
    }
    cout << "max level: " << m.level() << '\n';
}

int main()
{
    //using SkipList = myst::SkiplistMap<int, string, less<int>>;
    using SkipList = myst::SkiplistMultimap<int, string, less<int>>;
    try {
        SkipList st = { {10, "ten"}, {50, "five"}, {80, "eight"}, {40, "four"},
            {30, "three"}, {90, "nine"}, {60, "six"}, {20, "two"}, {70, "seven"} };
        
        // insert duplicates
        st.insert(50, "five");
        st.insert(60, "six");
        st.insert(60, "six");
        st.insert(60, "six");
        st.insert(60, "six");
        st.insert(60, "six");
        // only for map
        //st.insert_or_assign(60, "six * six");
        //st[60] = "six * six";

        print_map("st:\n", st);

        SkipList st2 = st;
        cout << "\n\nst2=st, in reverse order:\n";
        for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
            cout << '{' << i->first << ", " << i->second << "} : "
                << st.level(--i.base()) << '\n';
        }
        cout << "max level: " << st2.level() << '\n';

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);
        cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        print_map("", st);

        myst::swap(st, st2);
        print_map("\n\nst, after swapping with st2: \n", st);

        print_map("\n\nst2, after swapping with st: \n", st2);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
