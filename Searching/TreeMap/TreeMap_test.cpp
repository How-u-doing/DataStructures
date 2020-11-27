#include "TreeMap.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace mySymbolTable;

// ./TreeMap_test tinyST.txt
// ./TreeMap_test tinyTale.txt
// ./TreeMap_test tale.txt
int main(int argc, char* argv[])
{
	string filename{ argv[1] };
	ifstream ifs{ filename };
	if (!ifs.is_open()) cerr << "Error opening file " << filename << endl;
	try {
		TreeMap<char, int> st{};
		char key{};
		for (int i = 0; ifs >> key; ++i) {
			st[key] = i; // st.insert_or_assign(key, i);
		}

		st.print();

		cout << "Ordered traversal:\n";
		for (const auto& x : st)
			cout << x << "  ";

		cout << "\n\nst.select(7) = " << *st.select(7) << "\tst.select(3) = " << *st.select(3)
			<< "\nst.rank('r') = " << st.rank('r') << "\tst.rank('e') = " << st.rank('e') << '\n';

		cout << "st.floor('k') = " << *st.floor('k') << "\tst.floor('f') =" << *st.floor('f') << '\n'
			<< "st.ceiling('t') = " << *st.ceiling('t') << "\tst.ceiling('j') = " << *st.ceiling('j') << '\n';

		st.erase('e');	//st.erase('s');
		TreeMap<char, int> st2{ st };
		cout << "\nAfter removing 'e':\n";
		st2.print();
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}

	return 0;
}