#include "lcs.h"
#include <iostream>

using namespace std;

#define highlight_print(c) \
            cout << "\033[0;31m" << c << "\033[0m"

int main()
{
    string X, Y;
    cin >> X >> Y;
    auto matches = longest_common_subsequence(X, Y);
    const int t = matches.size();
    if (t == 0) {
        return 0;
    }

    const int m = X.size();
    for (int i = 0, k = 0; i < m; ++i) {
        if (k < t && i == matches[k].i) {
            highlight_print(X[i]);
            ++k;
        }
        else
            cout << X[i];
    }
    cout << '\n';

    const int n = Y.size();
    for (int j = 0, k = 0; j < n; ++j) {
        if (k < t && j == matches[k].j) {
            highlight_print(Y[j]);
            ++k;
        }
        else
            cout << Y[j];
    }
    cout << '\n';

    return 0;
}
