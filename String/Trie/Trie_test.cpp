#include "Trie.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace myTrie;
	std::ios_base::sync_with_stdio(false);

	if (argc < 2) { cerr << "lack of filename" << endl; return 1; }
	string filename{ argv[1] };
	ifstream ifs{ filename };
	if (!ifs.is_open()) { cerr << "Error opening file " << filename << endl; return 2; }
	try {
		TrieST<int> trie;
		int i = 0;
		for (string word; ifs >> word; ) {
			trie[word] = i++; // trie.insert_or_assign(word, i++);
		}
		
		for (const auto& word : trie.keys()) {
			cout << word << " : " << trie.at(word) << '\n';
		}

		cout << "\nlongest prefix of \"shellsort\": ";
		cout << trie.longest_prefix_of("shellsort") << '\n';
		cout << "longest prefix of \"quicksort\": ";
		cout << trie.longest_prefix_of("quicksort") << '\n';

		cout << "\nkeys with prefix \"sh\": ";
		for (const auto& word : trie.keys_with_prefix("sh")) {
			cout << word << "  ";
		}

		cout << "\n\nkeys that match \"?he?l?\": ";
		for (const auto& word : trie.keys_that_match("?he?l?")) {
			cout << word << "  ";
		}

		cout << "\n\nkeys that match \"s??\": ";
		for (const auto& word : trie.keys_that_match("s??")) {
			cout << word << "  ";
		}
		cout << '\n';

		cout << "\nAfter removing \"shells\": ";
		trie.erase("shells");
		cout << '\n';
		for (const auto& word : trie.keys()) {
			cout << word << " : " << trie.at(word) << '\n';
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