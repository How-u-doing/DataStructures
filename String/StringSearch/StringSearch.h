#ifndef STRINGSEARCH_H
#define STRINGSEARCH_H 1
#include <string>
#include <vector>

namespace myStringAlgo {

    std::vector<int> BF_search(const std::string& pat, const std::string& txt);
    std::vector<int> FL_search(const std::string& pat, const std::string& txt);

    std::vector<int> build_prefix_table(const std::string& P);

    std::vector<int> KMP_search(const std::string& pat, const std::string& txt);

    std::vector<std::vector<int>> build_transition_table(const std::string& P, int R = 256);

    std::vector<int> finite_automaton_search(const std::string& pat, const std::string& txt);

    std::vector<int> BM_search(const std::string& pat, const std::string& txt);
}

#endif // !STRINGSEARCH_H
