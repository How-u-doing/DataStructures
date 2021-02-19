#include "../SkiplistSet.h"
#include <iostream>

using namespace std;
namespace myst = mySymbolTable;

template<typename Set>
void print_set(std::string_view comment, const Set& s)
{
    std::cout << comment;
    for (auto i = s.begin(); i != s.end(); ++i) {
        cout << *i << " : " << s.level(i) << '\n';
    }
    cout << "max level: " << s.level() << '\n';
}

int main()
{
    //using SkipList = myst::SkiplistSet<int, less<int>>;
    using SkipList = myst::SkiplistMultiset<int, less<int>>;
    try {
        SkipList st = { 10,50,80,40,30,90,60,20,70 };
        // insert duplicates 
        st.insert(50);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        st.insert(60);
        
        print_set("st:\n", st);

        SkipList st2 = st;
        cout << "\n\nst2=st, in reverse order:\n";
        for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
            cout << *i << " : " << st.level(--i.base()) << '\n';
        }
        cout << "max level: " << st2.level() << '\n';

        size_t count1 = st.erase(50);
        size_t count2 = st.erase(60);

        cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
            << count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

        print_set("", st);

        myst::swap(st, st2);
        print_set("\n\nst, after swapping with st2: \n", st);

        print_set("\n\nst2, after swapping with st: \n", st2);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
