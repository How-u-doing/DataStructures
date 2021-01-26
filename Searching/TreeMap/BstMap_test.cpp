#include "BstMap.h"
#include <string>
#include <iostream>

using namespace std;
using namespace mySymbolTable;

int main()
{
	//using BST = BstMap<int, string, less<int>>;
	using BST = BstMultimap<int, string, less<int>>;
	try {
		BST st;
		st.insert(10, "ten");
		st.insert(50, "five");
		st.insert(80, "eight");
		st.insert(40, "four");
		st.insert(30, "three");
		st.insert(90, "nine");
		st.insert(60, "six");
		st.insert(20, "two");
		st.insert(70, "seven");
		
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

		cout << "st:\n"; 
		for (auto it : st) {
			cout << it << "  ";
		}
		cout << "\n\nst printed by lines: \n";
		st.print();

		BST st2 = st;
		cout << "\n\nst2=st, in reverse order:\n";
		for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
			cout << *i << "  ";
		}

		size_t count1 = st.erase(50);
		size_t count2 = st.erase(60);
		cout << "\n\nst, after removing 50 and 60: \n" << "there are \" "
			<< count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

		for (auto it : st) {
			cout << it << "  ";
		}
		cout << "\n\n";
		st.print();

		swap(st, st2);
		cout << "\n\nst, after swapping with st2: \n";
		for (auto it : st) {
			cout << it << "  ";
		}
		cout << "\n\n";
		st.print();

		/*cout << "\n\nst2, after swapping with st: \n";
		for (auto it : st2) {
			cout << it << "  ";
		}
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
