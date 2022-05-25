#include "matrix_chain_multiplication.h"
#include <climits>

// top-down, memoized lookup
ull matrix_chain_lookup(const std::vector<int>& p, int i, int j,
                        std::vector<std::vector<ull>>& m,
                        std::vector<std::vector<int>>& s)
{
    // already memoized (in table m)
    if (m[i][j] < ULLONG_MAX)
        return m[i][j];

    if (i == j) {
        m[i][j] = 0;
    }
    else {
        // lookup(p, 0, n-1) costs O(n^3) time because we will have one loop
        // for k = 0 to n-1, and each lookup for m[i][k] and m[k+1][j] will
        // cost O(1) time. Thus, total runtime is O(n) x O(n^2) (to compute
        // half of the table m for each entry j > i).
        for (int k = i; k < j; ++k) {
            ull cost = matrix_chain_lookup(p, i, k, m, s) +
                       matrix_chain_lookup(p, k+1, j, m, s) +
                       p[i] * p[k+1] * p[j+1]; // # of rows, cols, cols of
                                               // A_{i}, A_{k}, A_{j}
            if (cost < m[i][j]) {
                m[i][j] = cost;
                s[i][j] = k;
            }
        }
    }
    return m[i][j];
}

/*
 * Args:
 *   p: the dimension vector <p_0, p_1, ..., p_n>, assuming that matrix A_i
 *      has dimensions p_{i} x p_{i+1}, 0 <= i <= n-1.
 *
 *   m: m[i][j] := minimum number of multiplications required to perform the
 *      matrix multiplications A_{i} A_{i+1} ... A_{j}, for j > i.
 *
 *   s: each entry in the table s[0..n-2, 1..n-1] records which index of k
 *      achieved the optimal cost in computing m[i][j].
 *
 * The formula is simple:
 *
 *              / 0, if i == j
 *   m[i][j] = |
 *              \ min{ m[i][k] + m[k+1][j] + p_{i} x p_{k+1} x p_{j+1}, over all i<=k<j }
 *                                            ^       ^         ^
 *                                            |       |         |
 *   (A_{i}..A_{k}) (A_{k+1}..A_{j}):   # of raws  # of cols  # of cols
 *
 * Runs in O(n^3) time.
 *
 * See also https://en.wikipedia.org/wiki/Matrix_chain_multiplication or
 * section #15.2 matrix-chain multiplication in 'CLRS 3e'.
 */
void optimal_matrix_chain_order(const std::vector<int>& p,
                                std::vector<std::vector<ull>>& m,
                                std::vector<std::vector<int>>& s)
{
    int n = p.size() - 1;
    m = std::vector<std::vector<ull>>(n, std::vector<ull>(n, ULLONG_MAX));
    s = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
#ifndef MATRIX_CHAIN_BOTTOM_UP
    matrix_chain_lookup(p, 0, n-1, m, s);
#else
    // The bottom-up version is not as straightforward as the top-down one.
    // The observation is that to compute m[i][j] we will only need matrices
    // A_{i}..A_{j}, i.e. only j-i+1 matrices are required. So, we can use
    // a chain length variable `len` and consider the subproblem size to be
    // the length j-i+1 of the chain. If this doesn't make sense yet, please
    // refer to Figure 15.5 in 'CLRS 3e'.
    for (int i = 0; i < n; ++i)
        m[i][i] = 0;

    for (int len = 2; len <= n; ++len) { // length of the chain
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k) {
                ull cost = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }
#endif
}

/*
 * `print_optimal_parens(s, i, j)` adds parentheses for the matrix chain.
 * For example, print_optimal_parens(s, 0, 5) may print something like
 * ((A_{0}(A_{1}A_{2}))((A_{3}A_{4})A_{5})).
 * After parenthesization, we can use a stack to parse and evaluate the
 * printed string :)
 */
void print_optimal_parens(const std::vector<std::vector<int>>& s,
                          int i, int j)
{
    if (i == j) {
        std::cout << "A_{" << i << '}';
    }
    else {
        std::cout << '(';
        print_optimal_parens(s, i, s[i][j]);
        print_optimal_parens(s, s[i][j]+1, j);
        std::cout << ')';
    }
}
