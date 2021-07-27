#include "../RbMap.h"
#include <string>
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

template<typename Map>
void print_map(std::string_view comment, const Map& m)
{
    std::cout << comment;
    for (const auto& [key, value] : m) {
        std::cout << '{' << key << ", " << value << "} ";
    }
}

int main()
{
    //using RBT = myst::RbMap<int, string, less<int>>;
    using RBT = myst::RbMultimap<int, string, less<int>>;
    try {
        RBT st = { {10, "ten"}, {50, "five"}, {80, "eight"}, {40, "four"},
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
        cout << "\n\nst printed by lines: \n";
        st.print();
        cout << "height: " << st.height() << '\n';

        RBT st2 = st;
        cout << "\n\nst2=st, in reverse order:\n";
        for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
            cout << '{' << i->first << ", " << i->second << "} ";
        }

        st.insert(st.find(70), { 65, "sixty five" });
        cout << "\n\n";
        st.print();

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);
#ifndef NDEBUG
        if (!st.is_rb_tree()) {
            std::cout << "Not a red-black tree\n";
            return -1;
        }
#endif
        cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        print_map("", st);
        cout << "\n\n";
        st.print();
        cout << "height: " << st.height() << '\n';

        myst::swap(st, st2);
        print_map("\n\nst, after swapping with st2: \n", st);
        cout << "\n\n";
        st.print();
        cout << "height: " << st.height() << '\n';

        /*print_map("\n\nst2, after swapping with st: \n", st2);
        cout << "\n\n";
        st2.print();
        cout << "height: " << st2.height() << '\n';*/
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
