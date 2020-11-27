#define MYTREEMAP // uncomment to use std::map/std::unordered_map
#ifdef MYTREEMAP
#include "TreeMap.h"
#else
#include <unordered_map>
#endif // MYTREEMAP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // std::transform
#include <cctype>	 // ::tolower
#include <ctime>

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

	if (n <= 10) { // print all if no more then 10 items
		for (auto it = wordIters.cbegin(); it != sortedRangeEnd; ++it)
		{
			std::cout << (*it)->first << " :  " << (*it)->second << '\n';
			//std::printf("  %-10s%10zu\n", (*it)->first.c_str(), (*it)->second);
		}
	}
	else { // print first 5 and last 5 items
		auto it = wordIters.cbegin();
		for (int i = 0; i < 5; ++i, ++it)
			std::cout << (*it)->first << " :  " << (*it)->second << '\n';
		std::cout << "...\n";
		it = sortedRangeEnd - 5;
		for (int i = 0; i < 5; ++i, ++it)
			std::cout << (*it)->first << " :  " << (*it)->second << '\n';
	}	
}

// CLI: ./count_words leipzig1M.txt 8 10000
// test data file: https://algs4.cs.princeton.edu/31elementary/leipzig1M.txt
int main(int argc, char* argv[])
{
	using namespace std;
	
	try {
		clock_t start = clock();

		std::ios_base::sync_with_stdio(false);

		size_t n{};  // pick out words whose length are >= n
		size_t k{};  // show top k most common words
		if (argc < 2) { cerr << "Too few arguments" << endl; return 1; }
		else if (argc == 2) { n = 1; k = 10; }
		else if (argc == 3) { n = std::stoi(argv[2]); k = 10; }
		else { n = std::stoi(argv[2]); k = std::stoi(argv[3]); }
		string filename{ argv[1] };
		ifstream ifs{ filename };
		if (!ifs.is_open()) { cerr << "Error opening file " << filename << endl; return 2; }

#ifdef MYTREEMAP
		mySymbolTable::TreeMap<string, size_t> mp{};
#else
		unordered_map<string, size_t> mp{};
#endif // MYTREEMAP

		for (string word; ifs >> word;) {
			if (word.length() < n) continue; // dump short word 
			transform(word.begin(), word.end(), word.begin(), ::tolower); // note that tolower is of global namespace
			++mp[word];
		}
		
		show_most_common_words(mp.begin(), mp.end(), k);

		cout << "\nUsed " << (clock() - start) / (double)CLOCKS_PER_SEC <<
			"s to find the top " << k << " most common words" << endl;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}

	return 0;
}
