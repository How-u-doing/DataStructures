#include "myStringSearch.h"
#include <iostream>

using namespace std;

int main()
{
	string txt = "This is a test string. is is b This is a test string.";
	string pat = "is is a";
        
    //auto vi = BF_search(pat, txt);
    //auto vi = FL_search(pat, txt);
    auto vi = KMP_search(pat, txt);
    //auto vi = BM_search(pat, txt);

    for (const auto& x : vi)
        cout << x << endl;

	return 0;
}
