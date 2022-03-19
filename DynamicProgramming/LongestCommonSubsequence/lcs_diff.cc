#include "lcs.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define END    "\033[0m"

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " file1 file2\n";
        return 0;
    }

    ifstream file1{ argv[1] };
    ifstream file2{ argv[2] };
    if (!file1.is_open()) {
        cout << RED << "error: cannot open file " << argv[1] << '\n' << END;
        return 1;
    }
    if (!file2.is_open()) {
        cout << RED << "error: cannot open file " << argv[2] << '\n' << END;
        return 1;
    }

    // read the whole file into a string
    stringstream buffer1, buffer2;
    buffer1 << file1.rdbuf();
    string f1{ buffer1.str() };
    buffer2 << file2.rdbuf();
    string f2{ buffer2.str() };

    auto matches = longest_common_subsequence(f1, f2);
    if (matches.size() == 0) {
        cout << "no common subsequences\n";
        return 0;
    }

    const int t = matches.size();
    const int m = f1.size();
    const int n = f2.size();
    int i = 0, j = 0, k = 0;
    while (true) {
        // print common part
        while (k < t && f1[i] == f2[j]) {
            cout << f1[i];
            ++i; ++j;
            ++k;
        }

        // print old stuff in red
        cout << RED;
        while (i < m && i != matches[k].i) // until next new match
            cout << f1[i++];
        cout << END;

        // print new stuff in green
        cout << GREEN;
        while (j < n && j != matches[k].j)
            cout << f2[j++];
        cout << END;

        if (i == m && j == n)
            break;
    }

    return 0;
}
