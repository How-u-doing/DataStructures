#include "StringSearch.h"
#include <algorithm> // std::max, std::reverse_copy

namespace myStringAlgo {

/*
 * NB: In many practical uses (say, English text searches), the naive string
 * searching algorithm functions pretty efficient! See the README under the
 * same folder to verify it.
 *
 * For randomly chosen pattern P of length m and text T of length n, from the
 * d-ary alphabet, where d>=2, the expected number of character-to-character
 * comparisons is (see 'CLRS 3e' Exercise 32.1-3)
 *
 *           (n-m+1) * (1-d^{-m}) / (1-d^{-1}) <= 2(n-m+1).
 *
 * Proof:
 * Let E[k] be the expected # of character-to-character comparisons when the
 * pattern P is aligned with T[k:], where 0 <= k <= n-m.
 *
 * (For ease of writing, let p = 1/d, q = (d-1)/d = 1-p.)
 *
 * Then, E[k] = 1*q + 2*p*q + 3*p^2*q + ... + m*p^{m-1}*q + m*p^m
 *            = q * S + m*p^m,
 * where S = \sum_{i=1}^{m} i*p^{i-1}.
 *
 * Thus, q*S = (S-p*S) = p^0 + p^1 + ... + p^{m-1} - m*p^m.
 *
 * Therefore,
 *       E[k] = q*S + m*p^m = p^0 + p^1 + ... + p^{m-1}
 *            = (1-p^m) / (1-p)
 *            = (1-d^{-m}) / (1-d^{-1}).
 *
 * Since P and T are randomly chosen, E[k] for each shift k are the same, and
 * thereby giving the total expected # of character-to-character comparisons
 * (n-m+1) * E[k], which is the result given above.  QED
 */

// Brute-force string searching algorithm
std::vector<int> BF_search(const std::string& pat, const std::string& txt)
{
    const int M = pat.length();
    const int N = txt.length();
    std::vector<int> occurrences{};
    for (int i = 0; i <= N - M; ++i) {
        int j = 0;
        for (; j < M; ++j) {
            if (pat[j] != txt[i + j])
                break;
        }
        if (j == M)
            occurrences.push_back(i);
    }
    return occurrences;	// return an empty vector when no matches
}

// first-last matching
std::vector<int> FL_search(const std::string& pat, const std::string& txt)
{
    const int M = pat.length();
    const int N = txt.length();
    std::vector<int> occurrences{};
    for (int i = 0; i <= N - M; ++i) {
        int j = 0;
        int k = M - 1;
        for (; j <= k; ++j, --k) {
            // one more comparison at the middle one when M is odd
            if (pat[j] != txt[i + j] || pat[k] != txt[i + k])
                break;
        }
        if (j > k)
            occurrences.push_back(i);
    }
    return occurrences;
}

// k = pi[i] is the largest integer smaller than i such that
// P[0..k] is a suffix of P[0..i]
// pi[i] == -1 means no prefix of P is a suffix of P[0..i]
std::vector<int> build_prefix_table(const std::string& P)
{
    const int m = P.size();
    std::vector<int> pi(m, -1); // pi[0] = -1
    int k = -1;
    // Runs in Θ(m) time since
    //   1. pi[k] decreases k, k < i, pi[i] = k < i
    //   2. within the for loop k gets increased at most m-1 times
    //   3. when executing k = pi[k], 0 <= k < m
    // Therefore, the while body k = pi[k] is executed at most m-1 times in total.
    for (int i = 1; i < m; ++i) {
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
    std::vector<int> occurrences{};
    std::vector<int> pi = build_prefix_table(pat);
    int k = -1; // state of the finite automaton we have arrived
    for (int i = 0; i < n; ++i) { // O(n) complexity
        /* The idea here is we compare P[k+1] against the current char T[i],
         * if it is a match, increment the state (pointer) k; while it is a
         * mismatch, we keep falling back to the longest prefix of P[0..k]
         * that is also a suffix of P[0..k] by assigning k = pi[k] so that
         * we can align P[0..pi[k]] right with T[0..i-1], and then we test
         * the new position P[pi[k]+1] with T[i]. If there is no occurrence
         * of T[i] in P[0..k], we will fall back to the original state and
         * have to start from comparing at P[0] with T[i+1].
         */
        while (k > -1 && pat[k + 1] != txt[i])
            k = pi[k];
        if (pat[k + 1] == txt[i])
            ++k; // now points to the rightmost char matched
        if (k == m - 1) {
            occurrences.push_back(i - k);
            k = pi[k]; // align with next prefix (align pi[k] with k)
        }
    }
    return occurrences;
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
    std::vector<int> occurrences{};
    std::vector<std::vector<int>> delta = build_transition_table(pat);
    int q = 0;
    for (int i = 0; i < n; ++i) {
        q = delta[q][txt[i]];
        if (q == m)
            occurrences.push_back(i - m + 1);
    }
    return occurrences;
}

// bad_char[txt[i]] = shift of i such that the mismatched char at txt[i] can
// be aligned with the rightmost occurrence of txt[i] of pat. If no occurrence
// of txt[i] is found in pat, then align pat pass the mismatched char; if the
// rightmost occurrence of txt[i] is after j, that is, within the already
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
    std::vector<int> bad_char(R, m); // when no occurrence, shift == patlen
    for (int k = 0; k < m; ++k) {
        bad_char[P[k]] = m-1 - k;
    }
    return bad_char;
}

// pi_reversed[m-1-q] == m-1-(j+1), good_suffix[j] == m-q
// txt:  . . . . . . . . N L E . . .
//                         ===
// pat:  . . N L E N E E D L E
//             ===
// idx:        q   k     j
std::vector<int> build_good_suffix_table(const std::vector<int>& pi_reversed)
{
    const int m = pi_reversed.size();
    std::vector<int> good_suffix(m, -1);
    for (int q = 0, j; q < m-1; ++q) {
        j = pi_reversed[m-1-q]; // reverse index of j+1 in the above diagram
        if (j != -1)
            good_suffix[m-2-j] = m-q;
    }
    for (int j = 0; j < m-1; ++j) {
        // matched suffix doesn't reoccur
        if (good_suffix[j] == -1)
            good_suffix[j] = m-1 - j + m; // we can skip the whole pat
    }
    good_suffix[m-1] = 1;
    return good_suffix;
}

// Will pi_reversed[j] fall back to i?
bool fall_back(const std::vector<int>& pi_reversed, int j, int i)
{
    int k = pi_reversed[j];
    while (k > i)
        k = pi_reversed[k];
    return k == i;
}

// Boyer-Moore string-search algorithm:
// the standard benchmark for practical string-search literature
std::vector<int> BM_search(const std::string& pat, const std::string& txt)
{
    const int m = pat.length();
    const int n = txt.length();
    std::vector<int> occurrences{};
    std::vector<int> pi = build_prefix_table(pat); // shift pat by m-1 - pi[m-1] instead of
                                                   // just by 1 when found a complete match
    std::vector<int> bad_char = build_bad_char_table(pat);
    std::string pat_reversed(m, '\0');
    std::reverse_copy(pat.begin(), pat.end(), pat_reversed.begin());
    std::vector<int> pi_reversed = build_prefix_table(pat_reversed);

#define IMPLEMENTED_GOOD_SUFFIX 1

#if IMPLEMENTED_GOOD_SUFFIX
    std::vector<int> good_suffix = build_good_suffix_table(pi_reversed);
#endif
    int i = m - 1;
    while (i < n) {
        int j = m - 1;
        while (j >= 0 && pat[j] == txt[i]) {
            --i;
            --j;
        }
        if (j < 0) {
            occurrences.push_back(i + 1);
            i += m-1 - pi[m-1] + m;
        }
        else {
#if IMPLEMENTED_GOOD_SUFFIX
            i += std::max(bad_char[txt[i]], good_suffix[j]);
#else
            int shift = bad_char[txt[i]];
            if (shift > m-1 - j) { // rightmost occurrence of txt[i] is before j
                // We can potentially skip all matched characters by using the KMP
                // prefix table for the reversed pat. For example:
                // txt:  . . . . . . . . N L E . . .
                //                         ===
                // pat:  . . N L E N E E D L E
                //                   ===
                // idx:        q   k     j
                // If the matched characters pat[j+1..m-1] != pat[k+1..k+m-1-j], then
                // we can shift m-j more positions because the mismatched char txt[i]
                // isn't in already matched characters (rightmost occurrence of txt[i]
                // is before j). This can be quite helpful if m-j is large.
                // The good suffix rule tries to find the position q such that
                // pi_reversed[m-1-q] == m-1 - (j+1). But such q can be after k, which
                // means good suffix skip < bad char skip. So, here we test if q will
                // fall back on j+1 (i.e. q may go through many times of pi_reversed).
                // We can achieve this by decreasing k and test each such position,
                // but it is very inefficient. We can do better by pre-building the
                // good suffix table and memoize it.
#if 1
                // The fall_back (contains a loop) function call may slow down the
                // matching process in practice. For genomic substring search where
                // already matched characters can be huge this might be justified.
                if (shift < m && j < m - 1 && !fall_back(pi_reversed, shift-1, m-2-j))
                    shift += m-j;
#endif
            }
            else { // rightmost occurrence of txt[i] is after j
                // txt:     . . . . G A A T C G A A T A . . .
                // pat:       A T C T A A T C G A A T A
                shift = m-1 - j + m-2-j - pi_reversed[m-2-j];
            }
            i += shift;
#endif
        }
    }
    return occurrences;
}

} // namespace myStringAlgo
