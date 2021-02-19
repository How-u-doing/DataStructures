#include "../TST.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace mySymbolTable;
    std::ios_base::sync_with_stdio(false);

    if (argc < 2) { cerr << "lack of filename" << endl; return 1; }
    string filename{ argv[1] };
    ifstream ifs{ filename };
    if (!ifs.is_open()) { cerr << "Error opening file " << filename << endl; return 2; }
    try {
        TST<int> tst;
        int i = 0;
        for (string word; ifs >> word; ) {
            tst[word] = i++;    // tst.insert_or_assign(word, i++);
        }

        for (const auto& word : tst.keys()) {
            cout << word << " : " << tst.at(word) << '\n';
        }
        cout << "********************************\n"
            << "        backward print:\n"
            << "********************************\n";
        for (auto it = tst.crbegin(); it != tst.crend(); ++it) {
            /*const auto& [key, val] = *it;
            cout << key << " : " << val << '\n';*/
            cout << it.key() << " : " << it.val() << '\n';
        }

        cout << "\ntst print by lines (hide_null_links=false):\n";
        tst.print();
        cout << "height: " << tst.height() << '\n';

        cout << "\nlongest prefix of \"shellsort\": ";
        cout << tst.longest_prefix_of("shellsort") << '\n';
        cout << "longest prefix of \"quicksort\": ";
        cout << tst.longest_prefix_of("quicksort") << '\n';

        cout << "\nkeys with prefix \"sh\": ";
        for (const auto& word : tst.keys_with_prefix("sh")) {
            cout << word << "  ";
        }

        cout << "\n\nkeys that match \"?he?l?\": ";
        for (const auto& word : tst.keys_that_match("?he?l?")) {
            cout << word << "  ";
        }

        cout << "\n\nkeys that match \"s??\": ";
        for (const auto& word : tst.keys_that_match("s??")) {
            cout << word << "  ";
        }

        cout << '\n';
        cout << "\nReset last value to 100\n";
        auto iter = tst.rbegin();
        iter.val() = 100; // reset
        cout << iter.key() << " : " << iter.val() << '\n';

        tst.erase("shells");
        cout << "\nAfter removing \"shells\" (hide_null_links=true):\n\n";

        tst.print(/*hide_null_links=*/true);
        cout << "height: " << tst.height() << '\n';
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    catch (...) {
        cout << "Some unknown error happened" << endl;
    }

    return 0;
}
