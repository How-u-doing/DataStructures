#include "BST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace myBST;

//struct City {
//	string name;
//	size_t population;
//	double GDP;
//};

int main(int argc, char* argv[])
{
	//BST<string, City> bst{};
	string filename{ argv[1] };
	ifstream ifs{ filename };
	if (!ifs.is_open()) cerr << "Error opening file " << filename << endl;
	try {
		BST<char, int> bst{};
		char key{};
		for (auto i = 0; ifs >> key; ++i) {
			bst[key] = i; // bst.insert_or_assign(key, i);
		}

		vector<char> vc{};
		bst.level_order(vc);
		for (auto key : vc) {
			cout << key << '\t' << bst[key] << '\n';
		}

		cout << "bst.select(7)=" << bst.select(7) << "\tbst.select(3)=" << bst.select(3)
			<< "\nbst.rank('R')=" << bst.rank('R') << "\tbst.rank('E')=" << bst.rank('E') << '\n';

		cout << "bst.min()=" << bst.min() << "\tbst.max()=" << bst.max() << '\n'
			<< "bst.floor('K')=" << bst.floor('K') << "\tbst.floor('F')=" << bst.floor('F') << '\n'
			<< "bst.ceiling('T')=" << bst.ceiling('T') << "\tbst.ceiling('J')=" << bst.ceiling('J') << '\n';

		bst.erase('E');
		//bst.erase('M');
		//bst.erase_max();
		BST<char, int> bst2{ bst };
		vector<char> vc2{};
		bst.level_order(vc2);
		cout << "After removing 'E':\n";
		for (auto key : vc2) {
			cout << key << '\t' << bst2[key] << '\n';
		}
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}

	return 0;
}