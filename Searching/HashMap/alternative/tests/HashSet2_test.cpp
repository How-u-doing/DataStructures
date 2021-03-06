#include "../HashSet2.h"
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

struct myhash {
    size_t operator()(int x) const {
        return x % 13;
    }
};

int main()
{
    //using Hashtable = myst::alternative::HashSet<int/*, myhash*/>;
    using Hashtable = myst::alternative::HashMultiset<int, myhash>;
    try {
        Hashtable st  = { 10,50,80,40,30,90,60,20,70 };

        // insert duplicates
        st.insert(50);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);

        /*st.max_load_factor(0.5);
        st.insert(30);
        st.insert(30);
        st.insert(80);*/

        cout << "st:\n"; 
        for (auto it : st) {
            cout << it << "  ";
        }
        std::cout << "\n\n";
        st.print();

        Hashtable st2 = st;

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);
        cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        for (auto it : st) {
            cout << it << "  ";
        }
        std::cout << "\n\n";
        st.print();

        myst::alternative::swap(st, st2);
        cout << "\n\nst, after swapping with st2: \n";
        for (auto it : st) {
            cout << it << "  ";
        }
        std::cout << "\n\n";
        st.print();

        /*cout << "\n\nst2, after swapping with st: \n";
        for (auto it : st2) {
            cout << it << "  ";
        }
        std::cout << "\n\n";
        st.print();*/
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}