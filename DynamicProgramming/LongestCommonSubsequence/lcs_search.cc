#include "lcs.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define BLUE   "\033[0;34m"
#define END    "\033[0m"

#define highlight_print(c) \
            cout << RED << c << END

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " pattern\n";
        return 0;
    }
    string pat{ argv[1] };
    vector<string> text;
    for (string line; getline(cin, line); )
        text.push_back(line);

    for (size_t k = 0; k < text.size(); ++k) {
        const string& line = text[k];
        auto matches = longest_common_subsequence(pat, line);
        if (matches.size() < pat.size())
            continue;

        // follow color scheme of grep
        cout << GREEN << k+1 << END << BLUE << ':' << END;
        const int n = line.size();
        const int m = matches.size();
        for (int j = 0, k = 0; j < n; ++j) {
            if (k < m && j == matches[k].j) {
                highlight_print(line[j]);
                ++k;
            }
            else
                cout << line[j];
        }
        cout << '\n';
    }

    return 0;
}
