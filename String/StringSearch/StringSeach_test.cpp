#include "myStringSearch.h"
#include <iostream>

using namespace std;

int main()
{
	/*string txt = "cdabcdaabcabcdaabcab.";
	string pat = "abcdaabcab";*/
    string txt = "This is a test string. is is b This is a test string.";
    string pat = "is is a";
    cout << "text: \"" << txt << "\"\npattern: \"" << pat << "\"\n";
    
    cout << "\nby brute-force searching algorithm:\n";    
    auto vi1 = BF_search(pat, txt);
    for (const auto& x : vi1)
        cout << x << endl;

    cout << "\nby Index_FL searching algorithm:\n";
    auto vi2 = FL_search(pat, txt);
    for (const auto& x : vi2)
        cout << x << endl;

    cout << "\nby KMP string-searching algorithm:\n";
    auto vi3 = KMP_search(pat, txt);
    for (const auto& x : vi3)
        cout << x << endl;

    cout << "\nby Boyer-Moore string-search algorithm:\n";
    auto vi4 = BM_search(pat, txt);
    for (const auto& x : vi4)
        cout << x << endl;

    cout << "\n\n\nPress any key to leave...\n";
    char wait;
    cin >> noskipws >> wait;
	return 0;
}
