#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <queue>

namespace myStringAlgo {

typedef unsigned char UChar;

constexpr int Radix = 256;

struct ac_result_t {
    int pattern_index;
    int match_begin;
    int match_end;
};

struct ACTrieNode {
    ACTrieNode() {}
    ACTrieNode *goto_[Radix]{};
    ACTrieNode *failure_link_ = nullptr;
    int pat_idx_ = -1;
    bool is_word_ = false;  // is this node the last character of some pattern word?
};

class ACTrie {
    ACTrieNode *root_;  // the 0 state
    std::vector<std::string_view> patterns_;

    void init_root() { root_ = new ACTrieNode(); }

public:
    ACTrie(const std::string *patterns, size_t num) : patterns_(patterns, patterns + num) {
        init_root();
        for (size_t i = 0; i < num; i++) {
            insert(patterns[i].c_str(), i);
        }
        construct_failure_links();
    }

    ACTrie(const std::vector<std::string> &patterns) : ACTrie(patterns.data(), patterns.size()) {}

    ~ACTrie() { destroy(root_); }

    // get only the longest pattern if mutiple patterns have the same suffix
    // search "unix is awesome" for "nix|unix|nix is" -> ["unix", "nix is"]
    std::vector<ac_result_t> find(const char *haystack) {
        std::vector<ac_result_t> results;
        const UChar *h = reinterpret_cast<const UChar *>(haystack);
        ACTrieNode *n = root_;
        for (int pos = -1; *h; h++) {
            pos++;
            while (n && !n->goto_[*h]) {
                n = n->failure_link_;
            }
            if (!n) {
                n = root_;
                continue;
            }
            n = n->goto_[*h];
            if (n->is_word_) {
                int pattern_index = n->pat_idx_;
                int match_end = pos + 1;
                int match_begin = match_end - (int)patterns_[n->pat_idx_].size();
                results.push_back({pattern_index, match_begin, match_end});
            }
        }
        return results;
    }

    // search "unix is awesome" for "nix|unix|nix is" -> ["unix", "nix", "nix is"]
    std::vector<ac_result_t> find_all(const char *haystack) {
        std::vector<ac_result_t> results;
        const UChar *h = reinterpret_cast<const UChar *>(haystack);
        ACTrieNode *n = root_;
        for (int pos = -1; *h; h++) {
            pos++;
            while (n && !n->goto_[*h]) {
                n = n->failure_link_;
            }
            if (!n) {
                n = root_;
                continue;
            }
            n = n->goto_[*h];
            for (ACTrieNode *match_node = n; match_node->is_word_;
                 match_node = match_node->failure_link_) {
                int pattern_index = match_node->pat_idx_;
                int match_end = pos + 1;
                int match_begin = match_end - (int)patterns_[match_node->pat_idx_].size();
                results.push_back({pattern_index, match_begin, match_end});
            }
        }
        return results;
    }

private:
    void destroy(ACTrieNode *x) {
        if (x) {
            for (int c = 0; c < Radix; c++) {
                destroy(x->goto_[c]);
            }
        }
        delete x;
    }

    void insert(const char *pattern, size_t pattern_index) {
        if (!pattern) {
            return;
        }
        const UChar *c = reinterpret_cast<const UChar *>(pattern);
        ACTrieNode *cur = root_;
        while (*c) {
            if (cur->goto_[*c]) {
                cur = cur->goto_[*c];
            } else {
                ACTrieNode *node = new ACTrieNode;
                cur->goto_[*c] = node;
                cur = node;
            }
            c++;
        }
        cur->is_word_ = true;
        cur->pat_idx_ = pattern_index;
    }

    void construct_failure_links() {
        std::queue<ACTrieNode *> Q;
        // init first-level failure links
        for (int c = 0; c < Radix; c++) {
            ACTrieNode *child = root_->goto_[c];
            if (child) {
                Q.push(child);
                child->failure_link_ = root_;
            }
        }
        // Note that KMP is a special case of Aho-Corasick with one pattern.
        // BFS
        while (!Q.empty()) {
            ACTrieNode *cur = Q.front();
            Q.pop();
            for (int c = 0; c < Radix; c++) {
                ACTrieNode *child = cur->goto_[c];
                if (child) {
                    Q.push(child);
                    ACTrieNode *state = cur->failure_link_;
                    // only root_->failure_link_ == nullptr
                    while (state && !state->goto_[c]) {
                        state = state->failure_link_;
                    }
                    if (state) {
                        child->failure_link_ = state->goto_[c];
                    } else {
                        child->failure_link_ = root_;
                    }
                }
            }
        }
    }
};

}  // namespace myStringAlgo
