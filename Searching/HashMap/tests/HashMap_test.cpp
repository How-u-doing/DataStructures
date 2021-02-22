#include "../HashMap.h"
#include <string>
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

struct myhash {
    size_t operator()(int x) const {
        return x % 13;
    }
};

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
    //using Hashtable = myst::HashMap<int, string/*, myhash*/>;
    using Hashtable = myst::HashMultimap<int, string/*, myhash*/>;
    try {
        Hashtable st = { {10, "ten"}, {50, "five"}, {80, "eight"}, {40, "four"},
            {30, "three"}, {90, "nine"}, {60, "six"}, {20, "two"}, {70, "seven"} };
        
        // insert duplicates
        st.insert(50, "five * 1");
        st.insert(60, "six * 1");
        st.insert(60, "six * 2");
        st.insert(60, "six * 3");
        st.insert(60, "six * 4");
        st.insert(60, "six * 5");
        // only for map
        /*st.insert_or_assign(60, "six * six");
        st[60] = "six * six";*/

        print_map("st:\n", st);        
        cout << "\n\n";
        st.print();

        Hashtable st2 = st;

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);
        cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        print_map("", st);
        cout << "\n\n";
        st.print();

        myst::swap(st, st2);
        print_map("\n\nst, after swapping with st2: \n", st);
        cout << "\n\n";
        st.print();

        /*print_map("\n\nst2, after swapping with st: \n", st2);
        cout << "\n\n";
        st2.print();*/
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
