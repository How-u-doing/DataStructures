#include "BST.h"
//#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // std::transform
#include <locale>	 // std::tolower

// by Scott Meyers
template<typename MapIt>
void show_most_common_words(MapIt begin, MapIt end, size_t n)
{
	std::vector<MapIt> wordIters;
	wordIters.reserve(std::distance(begin, end));
	for (auto i = begin; i != end; ++i)
		wordIters.push_back(i);

	auto sortedRangeEnd = wordIters.begin() + n;
	std::partial_sort(wordIters.begin(), sortedRangeEnd, wordIters.end(),
		[](MapIt it1, MapIt it2) { return it1->second > it2->second; });

	for (auto it = wordIters.cbegin(); it != sortedRangeEnd; ++it)
	{
		std::printf("  %-10s%10zu\n", (*it)->first.c_str(), (*it)->second);
	}
}

// CLI: ./count_words leipzig1M.txt 8
// test data file: https://algs4.cs.princeton.edu/31elementary/leipzig1M.txt
int main(int argc, char* argv[])
{
	using namespace std;
	using namespace myBST;

	size_t n{}; // pick out words whose length is >= n
	if (argc < 2) { cerr << "To few arguments" << endl; return 1; }
	else if (argc == 2) n = 1;
	else if (argc == 3) n = std::stoi(argv[2]);
	string filename{ argv[1] };
	ifstream ifs{ filename };
	if (!ifs.is_open()) { cerr << "Error opening file " << filename << endl; return 2; }
	try {
		unordered_map<string, size_t> mp{};
		//BST<string, size_t> mp{};
		for (string word; ifs >> word;) {
			if (word.length() < n) continue; // dump short word
			transform(word.begin(), word.end(), word.begin(), tolower); // to lowercase
			++mp[word];
		}
		
		show_most_common_words(mp.begin(), mp.end(), 10);
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}

	return 0;
}