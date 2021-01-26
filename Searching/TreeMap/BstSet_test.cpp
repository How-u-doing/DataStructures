#include "BstSet.h"
#include <iostream>

using namespace std;
using namespace mySymbolTable;

int main()
{
	//using BST = BstSet<int, less<int>>;
	using BST = BstMultiset<int, less<int>>;
	try {
		BST st;
		st.insert(10);
		st.insert(50);
		st.insert(80);
		st.insert(40);
		st.insert(30);
		st.insert(90);
		st.insert(60);
		st.insert(20);
		st.insert(70);

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
