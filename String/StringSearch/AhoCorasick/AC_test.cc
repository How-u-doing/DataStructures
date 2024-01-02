#include "AhoCorasick.h"
#include <iostream>
#include <cassert>

using namespace myStringAlgo;
using namespace std;

void print_haystack(const char *haystack) {
    cout << "haystack:\n";
    int i = 0;
    for (const char *h = haystack; *h; h++) {
        cout << i % 10;
        i++;
    }
    cout << '\n';
    cout << haystack << '\n';
}

void print_patterns(const vector<string> &patterns) {
    cout << "patterns:\n";
    int i = 0;
    for (const auto &pat : patterns) {
        cout << "P[" << i++ << "] = " << pat << '\n';
    }
}

void print_matches(const vector<ac_result_t> &matches) {
    cout << "matches: \n";
    for (const auto &match : matches) {
        cout << match.pattern_index << ", " << match.match_begin << ", " << match.match_end << '\n';
    }
}

void print_test(const char *haystack, const vector<string> &patterns,
                const vector<ac_result_t> &matches) {
    print_haystack(haystack);
    print_patterns(patterns);
    print_matches(matches);
}

void test0() {
    const vector<string> patterns = {"apple", "maple", "snapple"};
    const char *haystack = "nobody likes maple in their apple flavored snapple";
    ACTrie ac(patterns);
    const auto matches = ac.find(haystack);
    print_test(haystack, patterns, matches);
}

void test1() {
    const vector<string> patterns = {"abcabababc", "cabababd", "abs"};
    const char *haystack = "abcabababs";
    ACTrie ac(patterns);
    const auto matches = ac.find_all(haystack);
    print_test(haystack, patterns, matches);
}

void test2() {
    const vector<string> patterns = {"nix", "unix", "nix is"};
    const char *haystack = "unix is awesome";
    ACTrie ac(patterns);
    const auto matches = ac.find_all(haystack);
    print_test(haystack, patterns, matches);
}

void test3() {
    // example from the Aho-Corasick paper
    const vector<string> patterns = {"he", "she", "his", "hers"};
    const char *haystack = "ushers";
    ACTrie ac(patterns);
    const auto matches = ac.find_all(haystack);
    print_test(haystack, patterns, matches);
}

int main() {
    test0();
    test1();
    test2();
    test3();
    return 0;
}
