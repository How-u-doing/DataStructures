#ifndef MATRIX_CHAIN_MULTIPLICATION_H
#define MATRIX_CHAIN_MULTIPLICATION_H

#include <vector>
#include <iostream>

typedef unsigned long long ull;

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
 * Runs in O(n^3) time.
 *
 * See also https://en.wikipedia.org/wiki/Matrix_chain_multiplication or
 * section #15.2 matrix-chain multiplication in 'CLRS 3e'.
 */
void optimal_matrix_chain_order(const std::vector<int>& p,
                                std::vector<std::vector<ull>>& m,
                                std::vector<std::vector<int>>& s);

/*
 * `print_optimal_parens(s, i, j)` adds parentheses for the matrix chain.
 * For example, print_optimal_parens(s, 0, 5) may print something like
 * ((A_{0}(A_{1}A_{2}))((A_{3}A_{4})A_{5})).
 * After parenthesization, we can use a stack to parse and evaluate the
 * printed string :)
 */
void print_optimal_parens(const std::vector<std::vector<int>>& s,
                          int i, int j);

#endif // MATRIX_CHAIN_MULTIPLICATION_H
