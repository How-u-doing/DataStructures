#include "BstSet.h"
#include <iostream>

using namespace std;
using namespace mySymbolTable;

int main()
{
	//using BST = BstSet<int, less<int>>;
	using BST = BstMultiset<int, less<int>>;
	try {
		BST st = { 10,50,80,40,30,90,60,20,70 };

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
		cout << "\n\nst printed by lines:\n";
		st.print();
		cout << "height: " << st.height() << '\n';

		BST st2 = st;
		cout << "\n\nst2=st, in reverse order:\n";
		for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
			cout << *i << "  ";
		}

		size_t count1 = st.erase(50);
		size_t count2 = st.erase(60);
		cout << "\n\nst, after removing 50 and 60: \n" << "there are \""
			<< count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

		for (auto it : st) {
			cout << it << "  ";
		}
		cout << "\n\n";
		st.print();
		cout << "height: " << st.height() << '\n';

		swap(st, st2);
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
