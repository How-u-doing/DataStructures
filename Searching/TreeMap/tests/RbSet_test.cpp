#include "../RbSet.h"
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

int main()
{
    //using RBT = myst::RbSet<int, less<int>>;
    using RBT = myst::RbMultiset<int, less<int>>;
    try {
        RBT st = { 10,50,80,40,30,90,60,20,70 };
        // insert duplicates 
        st.insert(50);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        
        cout << "st:\n"; 
        for (auto it : st) {
            cout << it << "  ";
        }
        cout << "\n\nst printed by lines: \n";
        st.print();
        cout << "height: " << st.height() << '\n';

        RBT st2 = st;
        cout << "\n\nst2=st, in reverse order:\n";
        for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
            cout << *i << "  ";
        }
        cout << '\n';

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);
#ifndef NDEBUG
        if (!st.is_rb_tree()) {
            std::cout << "Not a red-black tree\n";
            return -1;
        }
#endif
        cout << "\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        for (auto it : st) {
            cout << it << "  ";
        }
        cout << "\n\n";
        st.print();
        cout << "height: " << st.height() << '\n';

        myst::swap(st, st2);
        cout << "\n\nst, after swapping with st2: \n";
        for (auto it : st) {
            cout << it << "  ";
        }
        cout << "\n\n";
        st.print();
        cout << "height: " << st.height() << '\n';

        /*cout << "\n\nst2, after swapping with st: \n";
        for (auto it : st2) {
            cout << it << "  ";
        }
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
