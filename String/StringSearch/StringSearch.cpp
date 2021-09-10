#include "StringSearch.h"
#include <algorithm> // std::max, std::reverse_copy

namespace myStringAlgo {

// Brute-force string searching algorithm
std::vector<int> BF_search(const std::string& pat, const std::string& txt)
{
    const int M = pat.length();
    const int N = txt.length();
    std::vector<int> occurences{};
    for (int i = 0; i <= N - M; ++i) {
        int j = 0;
        for (; j < M; ++j) {
            if (pat[j] != txt[i + j])
                break;
        }
        if (j == M)
            occurences.push_back(i);
    }
    return occurences;	// return an empty vector when no matches
}

// first-last matching
std::vector<int> FL_search(const std::string& pat, const std::string& txt)
{
    const int M = pat.length();
    const int N = txt.length();
    std::vector<int> occurences{};
    for (int i = 0; i <= N - M; ++i) {
        int j = 0;
        int k = M - 1;
        for (; j <= k; ++j, --k) {
            // one more comparison at the middle one when M is odd
            if (pat[j] != txt[i + j] || pat[k] != txt[i + k])
                break;
        }
        if (j > k)
            occurences.push_back(i);
    }
    return occurences;
}

// k = pi[i] is the largest integer smaller than i such that
// P[0..k] is a suffix of P[0..i]
// pi[i] == -1 means no prefix of P is a suffix of P[0..i]
std::vector<int> build_prefix_table(const std::string& P)
{
    const int m = P.size();
    std::vector<int> pi(m, -1); // pi[0] = -1
    int k = -1;
    for (int i = 1; i < m; ++i) { // O(m) complexity
        while (k > -1 && P[k + 1] != P[i])
            k = pi[k]; // P[0..pi[k]] suff P[0..k] suff P[0..i]
        if (P[k + 1] == P[i])
            ++k;
        pi[i] = k;
    }
    return pi;
}

// Knuth-Morris-Pratt (KMP) string-searching algorithm
std::vector<int> KMP_search(const std::string& pat, const std::string& txt)
{
    const int m = pat.length();
    const int n = txt.length();
    std::vector<int> occurences{};
    std::vector<int> pi = build_prefix_table(pat);
    int k = -1; // state of the finite automaton we have arrived
    for (int i = 0; i < n; ++i) { // O(n) complexity
        /* The idea here is we compare P[k+1] against the current char T[i],
         * if it is a match, increment the state (pointer) k; while it is a
         * mismatch, we keep falling back to the longest prefix of P[0..k]
         * that is also a suffix of P[0..k] by assigning k = pi[k] so that
         * we can align P[0..pi[k]] right with T[0..i-1], and then we test
         * the new position P[pi[k]+1] with T[i]. If there is no occurence
         * of T[i] in P[0..k], we will fall back to the original state and
         * have to start from comparing at P[0] with T[i+1].
         */
        while (k > -1 && pat[k + 1] != txt[i])
            k = pi[k];
        if (pat[k + 1] == txt[i])
            ++k; // now points to the rightmost char matched
        if (k == m - 1) {
            occurences.push_back(i - k);
            k = pi[k]; // align with next prefix (align pi[k] with k)
        }
    }
    return occurences;
}

std::vector<std::vector<int>> build_transition_table(const std::string& P, int R)
{
    const int m = P.size();
    std::vector<std::vector<int>> delta(m + 1, std::vector<int>(R, 0));
#if 0
    // The 1st version is according to CLRS-3e Exercise 32.4-8:
    // delta(q,a)=delta(pi[q],a) if q=m or P[q+1]!=a (1-indexed)
    std::vector<int> pi = build_prefix_table(P); // 0-indexed
    // q=0, delta(0,c)=0 for all c, except delta(0,P[0])=1
    delta[0][P[0]] = 1;
    for (int q = 1; q <= m; ++q) {
        for (int c = 0; c < R; ++c)
            delta[q][c] = delta[pi[q-1] + 1][c];
        if (q < m)
            delta[q][P[q]] = q + 1;
    }
#else
    // The 2nd version avoids building the prefix table by
    // keeping track of pi[q] at the very beginning, which
    // can be seen as some sort of optimization. To update
    // pi[q], just note that pi[q+1]=delta(pi[q], P[q+1]).
    delta[0][P[0]] = 1;
    int k = 0; // pi[q] = k
    for (int q = 1; q <= m; ++q) {
        for (int c = 0; c < R; ++c)
            delta[q][c] = delta[k][c];
        if (q < m)
            delta[q][P[q]] = q + 1;
        k = delta[k][P[q]]; // k = pi[q+1]
    }
#endif
    return delta;
}

std::vector<int> finite_automaton_search(const std::string& pat, const std::string& txt)
{
    const int m = pat.length();
    const int n = txt.length();
    std::vector<int> occurences{};
    std::vector<std::vector<int>> delta = build_transition_table(pat);
    int q = 0;
    for (int i = 0; i < n; ++i) {
        q = delta[q][txt[i]];
        if (q == m)
            occurences.push_back(i - m + 1);
    }
    return occurences;
}

// bad_char[txt[i]] = shift of i such that the mismatched char at txt[i] can
// be aligned with the rightmost occurence of txt[i] of pat. If no occurence
// of txt[i] is found in pat, then align pat pass the mismatched char; if the
// rightmost occurence of txt[i] is after j, that is, within the already
// matched characters pat[j+1..patlen-1], then we will just move pat right by
// one position or it'll move backward (left)!  Note that, after the skip of
// pat, we also need to move i to pat's ending character position.
// In summary, we have delta1 = patlen-1 - j + pat_skip (= j-k) = patlen-1-k.
// txt:     . . . . N L E . . .
// pat:       N E E D L E
// idx:       k     j
std::vector<int> build_bad_char_table(const std::string& P)
{
    const int m = P.size(), R = 256;
    std::vector<int> bad_char(R, m); // when no occurence, shift == patlen
    for (int k = 0; k < m; ++k) {
        bad_char[P[k]] = m-1 - k;
    }
    return bad_char;
}

std::vector<int> build_good_suffix_table(const std::string& P)
{
    const int patlen = P.size();
    std::vector<int> good_suffix(patlen, 0);
    // to be accomplished...
    return good_suffix;
}

// Boyer-Moore string-search algorithm:
// the standard benchmark for practical string-search literature
std::vector<int> BM_search(const std::string& pat, const std::string& txt)
{
    const int m = pat.length();
    const int n = txt.length();
    std::vector<int> occurences{};
    std::vector<int> pi = build_prefix_table(pat); // shift pat by m-1 - pi[m-1] instead of
                                                   // just by 1 when found a complete match
    std::vector<int> bad_char = build_bad_char_table(pat);

#define IMPLEMENTED_GOOD_SUFFIX 0

#if IMPLEMENTED_GOOD_SUFFIX
    std::vector<int> good_suffix = build_good_suffix_table(pat);
#else
    std::string pat_reversed(m, '\0');
    std::reverse_copy(pat.begin(), pat.end(), pat_reversed.begin());
    std::vector<int> pi_reversed = build_prefix_table(pat_reversed);
#endif
    int i = m - 1;
    while (i < n) {
        int j = m - 1;
        while (j >= 0 && pat[j] == txt[i]) {
            --i;
            --j;
        }
        if (j < 0) {
            occurences.push_back(i + 1);
            i += m-1 - pi[m-1] + m;
        }
        else {
#if IMPLEMENTED_GOOD_SUFFIX
            i += std::max(bad_char[txt[i]], good_suffix[j]);
#else
            int shift = bad_char[txt[i]];
            if (shift > m-1 - j) { // rightmost occurence of txt[i] is before j
                // We can potentially skip all matched characters also by using the KMP
                // prefix table for the reversed pat. For example:
                // txt:     . . . . N L E . . .
                //                    ===
                // pat:       N E E D L E
                //              ===
                // idx:       k     j
                // As long as the matched characters pat[j+1..m-1] != pat[k+1..k+m-1-j],
                // then we can skip further all the matched characters, since there's no
                // txt[i] in the matched characters in this case (k < j).
                if (shift < m && j < m - 1 && pi_reversed[shift - 1] != m - 2 - j)
                    shift = m;
            }
            else { // rightmost occurence of txt[i] is after j
                // txt:     . . . . G A A T C G A A T A . . .
                // pat:       A T C T A A T C G A A T A
                shift = m-1 - j + m - 2 - j - pi_reversed[m - 2 - j];
            }
            i += shift;
#endif
        }
    }
    return occurences;
}

} // namespace myStringAlgo
