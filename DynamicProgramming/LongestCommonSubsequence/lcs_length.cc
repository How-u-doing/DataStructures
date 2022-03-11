#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int longest_common_subsequence(const string& X, const string& Y)
{
    const int m = X.size();
    const int n = Y.size();
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0)); // lengths of lcs

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (X[i] == Y[j])
                dp[i+1][j+1] = dp[i][j] + 1;
            else
                dp[i+1][j+1] = max(dp[i][j+1], dp[i+1][j]);

    return dp[m][n];
}

int main()
{
    string X, Y;
    cin >> X >> Y;
    int lcs = longest_common_subsequence(X, Y);
    cout << lcs << '\n';
    return 0;
}
