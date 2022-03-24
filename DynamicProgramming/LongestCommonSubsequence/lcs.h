#ifndef LCS_H
#define LCS_H

#include <string>
#include <vector>

struct Coord {
    int i;
    int j;
    Coord(int ii, int jj) : i(ii), j(jj) {}
};

/*
 * Runs in O(|X||Y|) time.
 * Denote prefixes X(i) := X[1..i], Y(i) := Y[1..i],
 * (X := X[1..m], Y := Y[1..n]) then we have
 *
 *                      /  null,                                      if i==0 or j==0
 *   LCS(X(i), Y(j)) = |   LCS(X(i-1), Y(j-1))^X[i],                  if i,j>0 && X[i] == Y[j],
 *                      \  max(LCS(X(i), Y(j-1)), LCS(X(i-1), Y(j))), if i,j>0 && X[i] != Y[j],
 *
 * where ^ denotes string concatenation.
 *
 * See also https://en.wikipedia.org/wiki/Longest_common_subsequence_problem.
 */
std::vector<Coord> longest_common_subsequence(const std::string& X, const std::string& Y);

#endif
