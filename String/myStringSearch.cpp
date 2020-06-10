#include "myStringSearch.h"
#include <cmath>

// Brute-force string search algorithm
std::vector<int> BF_search(std::string pat, std::string txt) 
{
	int M = pat.length();
	int N = txt.length();
	std::vector<int> vi{};

	for (int i = 0; i <= N - M; ++i) {
		int j = 0;
		for (; j < M; ++j) {
			if (pat[j] != txt[i + j])
				break;
		}
		if (j == M)
			vi.push_back(i);
	}

	return vi;	// return an empty vector when no matches
}

// first-last matching
std::vector<int> FL_search(std::string pat, std::string txt)
{
	int M = pat.length();
	int N = txt.length();
	std::vector<int> vi{};

	for (int i = 0; i <= N - M; ++i) {
		int j = 0;
		int mid = round(float(M) / 2);
		for (; j < mid; ++j) {// one more comparison at middle one when M is odd
			if (pat[j] != txt[i + j] || pat[M - 1 - j] != txt[i + M - 1 - j])
				break;
		}
		if (j == mid)
			vi.push_back(i);
	}

	return vi;	// return an empty vector when no matches
}


// Knuth–Morris–Pratt (KMP) string-searching algorithm
// see also <https://en.wikipedia.org/wiki/Knuth-Morris-Pratt_algorithm>
std::vector<int> KMP_search(std::string pat, std::string txt) 
{
	int m = pat.length();
	int n = txt.length();
	std::vector<int> vi;

	// preprocessing, to obtain "partial match" table pi
	int* pi = new int[m + 1]{ -1 };
	KMP_table(pi, pat, m);

	int j = 0;	// the position of the current character in txt
	int k = 0;  // the position of the current character in pat
	while (j < n) {
		if (pat[k] == txt[j]) {
			++k;
			++j;
			if (k == m) {// found
				vi.push_back(j - k);
				k = pi[k];	// to find next pattern starting at pi[k]
			}
		}
		else {// mismatch at some character
			k = pi[k];	// next match starting at pi[k]
			if (k < 0) {// mismatch at some character that is equal to pat[0]
				++j;
				++k;	// i.e. set k=0
			}
		}
	}

	return vi;
}

// failure function; see also 'CLRS-4e' P1002- 
void KMP_table(int pi[], std::string P, int m) 
{
	// <https://web.stanford.edu/class/cs97si/10-string-algorithms.pdf>
	// P21: pi^(k)[i] denotes pi[.] applied k times to i,
	// e.g. pi^(2)[i]=pi[pi[i]].
	// pi[i] is equal to pi^(k)[i-1] + 1, where k is the smallest integer
	// that satisfies P_{pi^(k)[i-1] + 1} = P_i.
	// If there no such k, pi[i]=0. 
	pi[0] = -1;
	for (int i = 1; i <= m; ++i) {
		int k = pi[i - 1];
		while (k >= 0 && P[k] != P[i - 1])
			k = pi[k];
		pi[i] = k + 1;
	}	
}


// Boyer-Moore string-search algorithm:
// the standard benchmark for practical string-search literature
std::vector<int> BM_search(std::string pat, std::string txt)
{

	return std::vector<int>();
}
