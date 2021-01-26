#include "_Tree.h"
#include <iostream>

using namespace std;
using namespace mySymbolTable;

int main(int argc, char* argv[])
{
	using tree = Tree<int, greater<int>>;
	/*using iterator = tree::iterator;
	using const_iterator = tree::const_iterator;*/

	tree st;
	st.insert(10);
	st.insert(50);
	st.insert(80);
	st.insert(40);
	st.insert(30);
	st.insert(90);
	st.insert(60);
	st.insert(20);
	st.insert(70);

	cout << "st:\n";
	for (auto it : st) {
		cout << it << "  ";
	}

	cout << "\n\nst printed by lines: \n";
	st.print();

	tree st2 = st;
	cout << "\n\nst2=st, in reverse order:\n";
	for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
		cout << *i << "  ";
	}

	st.erase(80);
	cout << "\n\nst, after removing 80: \n";
	for (auto it : st) {
		cout << it << "  ";
	}
	cout << "\n\n";
	st.print();
	return 0;
}