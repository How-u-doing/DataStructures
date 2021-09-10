#include "StringSearch.h"
#include <string>
#include <iostream>

using namespace std;
using namespace myStringAlgo;

int main()
{
#if 0
    string txt = ". . . G A A T C G A A T A . . .";
    string pat = "A T C T A A T C G A A T A";
#elif 0
    string txt = "CCABABABCAAAABAABABC";
    string pat = "ABAB";
#else
    string txt = "This is a test string. is is b This is a test string.is ws a";
    string pat = "is is a";
#endif

    cout << "txt: \"" << txt << "\"\npat: \"" << pat << "\"\n";
    
    cout << "\nby brute-force searching algorithm:\n";    
    auto res_BF = BF_search(pat, txt);
    for (const auto& x : res_BF)
        cout << x << '\n';

    cout << "\nby first-last searching algorithm:\n";
    auto res_FL = FL_search(pat, txt);
    for (const auto& x : res_BF)
        cout << x << '\n';

    cout << "\nby KMP string-searching algorithm:\n";
    auto res_KMP = KMP_search(pat, txt);
    for (const auto& x : res_KMP)
        cout << x << '\n';

    cout << "\nby finite automaton string-searching algorithm:\n";
    auto res_FA = finite_automaton_search(pat, txt);
    for (const auto& x : res_FA)
        cout << x << '\n';

    cout << "\nby Boyer-Moore string-search algorithm:\n";
    auto res_BM = BM_search(pat, txt);
    for (const auto& x : res_BM)
        cout << x << '\n';

    /*pat = "ababababacaaba";
    auto table = build_prefix_table(pat);
    for (auto x : table) {
        cout << x << " ";
    }
    cout << '\n';*/

    return 0;
}
