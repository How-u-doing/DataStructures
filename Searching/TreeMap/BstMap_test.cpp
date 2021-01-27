#include "BstMap.h"
#include <string>
#include <iostream>

using namespace std;
using namespace mySymbolTable;

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

		print_map("st:\n", st);		
		cout << "\n\nst printed by lines: \n";
		st.print();

		BST st2 = st;
		cout << "\n\nst2=st, in reverse order:\n";
		for (auto i = st2.rbegin(); i != st2.crend(); ++i) {
			cout << '{' << i->first << ", " << i->second << "} ";
		}

		size_t count1 = st.erase(50);
		size_t count2 = st.erase(60);
		cout << "\n\nst, after removing 50 and 60: \n" << "there are \" "
			<< count1 << "\" 50 and \"" << count2 << "\" 60 being removed\n";

		print_map("", st);
		cout << "\n\n";
		st.print();

		swap(st, st2);
		print_map("\n\nst, after swapping with st2: \n", st);
		cout << "\n\n";
		st.print();

		print_map("\n\nst2, after swapping with st: \n", st2);
		cout << "\n\n";
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
