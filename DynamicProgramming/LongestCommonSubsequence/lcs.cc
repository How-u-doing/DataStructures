#include "lcs.h"
#include <algorithm>

using namespace std;

vector<Coord> longest_common_subsequence(const string& X, const string& Y)
{
    const int m = X.size();
    const int n = Y.size();
    vector<vector<int>> lcs(m+1, vector<int>(n+1, 0));

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (X[i] == Y[j])
                lcs[i+1][j+1] = lcs[i][j] + 1;
            else
                lcs[i+1][j+1] = max(lcs[i][j+1], lcs[i+1][j]);

    // backtrack, in O(m+n) time
    int k = lcs[m][n];
    vector<Coord> matches(k, Coord(0, 0));
    if (k == 0) return matches;
    int i = m-1, j = n-1;
    while (i >= 0 && j >= 0) {
        if (X[i] == Y[j])
            matches[--k] = Coord(i--, j--);
        else if (lcs[i][j+1] > lcs[i+1][j])
            --i;
        else
            --j;
    }

    return matches;
}
