#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

typedef long long LL;

using namespace std;

struct Coord {
    int i;
    int j;
    Coord(int ii, int jj) : i(ii), j(jj) {}
};

stack<Coord> path;

/*
 * Given a m x n grid filled with non-negative numbers,
 * find a path from top left to bottom right, which
 * minimizes the sum of all numbers along its path.
 *
 * Note: You can only move either down or right at any
 *       point in time.
 *
 * For example, grid = [1 3 1;
 *                      1 5 1;
 *                      4 2 1]
 * The path 1->3->1->1->1 minimizes the sum ( = 7 ).
 * See also https://leetcode.com/problems/minimum-path-sum
 *
 * Easy to see that
 *      DP(i, j) = min(D(i, j-1), DP(i-1, j)) + g[i][j]
 * BCs: i = 0, DP(0, j) = DP(0, j-1) + g[0][j],
 *      j = 0, DP(i, 0) = DP(i-1, 0) + g[i][0],
 *      DP(0, 0) = g[0][0].
 */
LL min_path_sum(const vector<vector<int>>& grid)
{
    size_t m = grid.size();
    size_t n = grid[0].size();
    vector<vector<LL>> dp(m, vector<LL>(n, 0));
    dp[0][0] = grid[0][0];
    // not cache friendly :)
    for (size_t i = 1; i < m; ++i)
        dp[i][0] = dp[i-1][0] + grid[i][0];
    for (size_t j = 1; j < n; ++j)
        dp[0][j] = dp[0][j-1] + grid[0][j];

    for (size_t i = 1; i < m; ++i)
        for (size_t j = 1; j < n; ++j)
            dp[i][j] = min(dp[i][j-1], dp[i-1][j]) + grid[i][j];

    // write the min path, including g[0][0] and g[m-1][n-1]
    int i = m-1, j = n-1;
    while (i > 0 && j > 0) {
        if (dp[i][j-1] < dp[i-1][j])
            path.emplace(i, j--);
        else
            path.emplace(i--, j);
    }
    if (!(i == 0 && j == 0)) {
        while (i > 0)
            path.emplace(i--, 0);
        while (j > 0)
            path.emplace(0, j--);
        path.emplace(0, 0);
    }

    return dp[m-1][n-1];
}

int main()
{
    ios_base::sync_with_stdio(false);
    int m{}, n{};
    cin >> m >> n;
    vector<vector<int>> grid(m, vector<int>(n, 0));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cin >> grid[i][j];

    LL min_sum = min_path_sum(grid);
    auto pathcp = path;
    cout << min_sum << '\n';

    // output path
    while (!path.empty()) {
        Coord coord = path.top();
        cout << "(" << coord.i << ", " << coord.j << ")";
        if (path.size() != 1)
            cout << " -> ";
        path.pop();
    }
    cout << '\n';

    // print graph
    pathcp.pop(); // pop g[0][0]
    cout << grid[0][0];
    int i = 0, j = 0;
    while (!pathcp.empty()) {
        Coord coord = pathcp.top();
        if (coord.i != i) { // down, coord.i == i+1
            // assume each element occupies one space (' ')

            // print grid[i][j+1:]
            for (int k = j+1; k < n; ++k)
                cout << "    " << grid[i][k];
            cout << '\n';

            // print 'down' sign
            for (int k = 0; k < j; ++k)
                cout << "     ";
            cout << "|\n";
            for (int k = 0; k < j; ++k)
                cout << "     ";
            cout << "v\n";

            // print grid[i+1][0..j]
            for (int k = 0; k < j; ++k)
                cout << grid[i+1][k] << "    ";
            cout << grid[i+1][j];
        }
        else {
            cout << " -> " << grid[i][j+1];
        }

        i = coord.i;
        j = coord.j;
        pathcp.pop();
    }
    cout << '\n';

    return 0;
}
