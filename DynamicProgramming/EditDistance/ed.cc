#include "ed.h"
#include <vector>

using namespace std;

/*
 * Edit distance (often interchangeably with Levenshtein distance):
 * mimimum number of edits (ins, del, sub) required to turn the
 * source string into the target string.
 *
 * Runs in O(|S||T|) time.
 *
 * See also https://en.wikipedia.org/wiki/Edit_distance and
 *          https://en.wikipedia.org/wiki/Levenshtein_distance.
 *
 * d(i, j) := edit distance of prefixes S[1..i] and T[1..j]
 * d(0, j) := j (insertions), edit distance of inserting T[1..j]
 * d(i, 0) := i (deletions), edit distance of deleting S[1..i]
 *
 * d(i, j) := d(i-1, j-1),                             if S[i] == T[j],
 * d(i ,j) := min ( d(i-1, j) + w_del(S[i]),
 *                  d(i, j-1) + w_ins(T[j]),
 *                  d(i-1, j-1) + w_sub(S[i], T[j]) ), if S[i] != T[j].
 * Typically, in Levenshtein distance, cost of ins = del = sub = 1.
 * But we can always customize them for our own needs. For example,
 * we can use cost of substitution as 1 whereas cost of insertion
 * and deletion as 0.5.
 *
 * Also note that the longest common subsequence (LCS) distance can be
 * seen as a special case of edit distance, with cost of insertion and
 * deletion being 1, and cost of substitution being infinity. In other
 * words, only insertion and deletetion are allowed, not substitution.
 */
int edit_distance(const std::string& S, const std::string& T)
{
    const int m = S.size();
    const int n = T.size();
#if 0
    vector<vector<int>> d(m+1, vector<int>(n+1, 0));

    for (int i = 1; i <= m; ++i)
        d[i][0] = i;  // delete S[1..i]

    for (int j = 1; j <= n; ++j)
        d[0][j] = j;  // insert T[1..j]

    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            if (S[i-1] == T[j-1])
                d[i][j] = d[i-1][j-1];
            else
                d[i][j] = min( min(d[i-1][j] + 1,     // delete S[i]
                                   d[i][j-1] + 1),    // insert T[j]
                                   d[i-1][j-1] + 1 ); // subs S[i] -> T[j]
    return d[m][n];
#else
    // space-optimized version
    vector<int> d(n+1, 0);

    // d[0][j] = j
    for (int j = 1; j <= n; ++j)
        d[j] = j;  // insert T[1..j]

    for (int i = 1; i <= m; ++i) {
        d[0] = i;       // d[i][0]
        int prev = i-1; // d[i-1][0]
        for (int j = 1; j <= n; ++j) {
            int tmp = d[j];
            if (S[i-1] == T[j-1])
                d[j] = prev; // d[i][j] = d[i-1][j-1]
            else
                d[j] = min( min(d[j] + 1,     // delete S[i]
                                d[j-1] + 1),  // insert T[j]
                                prev + 1 );   // subs S[i] -> T[j]
            prev = tmp; // d[i-1][j-1]
        }
    }
    return d[n];
#endif
}
