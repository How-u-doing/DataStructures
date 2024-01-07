#include "StringSearch.h"
#include "AhoCorasick/AhoCorasick.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <ctime>

using namespace std;

struct match_t {
    int match_begin;
    int match_end;
};

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define BLUE   "\033[0;34m"
#define END    "\033[0m"

void print_help(const char* arg0)
{
    cout << "Usage: " << arg0 << " [OPTION...] PATTERNS FILE\n"
         << "Currently available options are:\n"
         << "  -h  (help)\n"
         << "        Output a usage message and exit.\n"
         << "  -c  (count)\n"
         << "        Print the count of the pattern in the file.\n"
         << "  -t  (time)\n"
         << "        Time how long it takes to find all the occurrences of the pattern.\n"
         << "  -m= (method)\n"
         << "        BM (Boyer-Moore string search algorithm),\n"
         << "        KMP (Knuth-Morris-Pratt search),\n"
         << "        FA (Finite Automaton search),\n"
         << "        BF (Brute-Force search).\n";
}

string method = "BM";

void parse_args_and_opts(int argc, char* argv[], vector<string>& args,
                         unordered_set<string>& options)
{
    const string opt_prefix = "-";
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            for (char* c = &argv[i][1]; *c != '\0'; ++c) {
                options.insert(opt_prefix + *c);
                // e.g. -m=KMP
                if (*c == 'm' && *(c+1)) {
                    method = c+2;
                    break;
                }
            }
        }
        else {
            args.push_back(argv[i]);
        }
    }
}

vector<int> search(const string& pattern, const string& text,
                   const string& method)
{
    if (method == "BM")
        return myStringAlgo::BM_search(pattern, text);
    else if (method == "KMP")
        return myStringAlgo::KMP_search(pattern, text);
    else if (method == "FA")
        return myStringAlgo::finite_automaton_search(pattern, text);
    else
        return myStringAlgo::BF_search(pattern, text);
}

void print_matches(const string& text, const vector<match_t>& matches)
{
    const int M = matches.size();
    const int T = text.size();
    int i = 0, k = 0, line = 1;
    int start_of_line = 0;
    while (k < M && i < T) {
        if (i == matches[k].match_begin) { // print the whole line
            // follow color scheme of grep
            cout << GREEN << line << END << BLUE << ':' << END;
            cout << text.substr(start_of_line, i - start_of_line);
            // add i < T in case of noeol
            while (i < T && text[i] != '\n') {
                if (i == matches[k].match_begin) {
                    int pat_len = matches[k].match_end - matches[k].match_begin;
                    cout << RED; // highlight matches
                    cout << text.substr(matches[k].match_begin, pat_len);
                    cout << END;
                    ++k;
                    i += pat_len;
                }
                else {
                    cout << text[i++];
                }
            }
            cout << '\n';
        }

        if (text[i] == '\n') {
            ++line;
            start_of_line = i + 1;
        }

        ++i;
    }
}

// "hello\|howdy\|hey there" -> ["hello", "howdy", "hey there"]
vector<string> split_partterns(const char *s) {
    vector<string> patterns;
    for (const char *p = s; *p;) {
        if (p[0] == '\\' && p[1] == '|') {
            if (p != s)
                patterns.emplace_back(s, p);
            s = p = p + 2;
            continue;
        }
        if (!p[1])
            patterns.emplace_back(s, p + 1);
        p++;
    }
    return patterns;
}

const unordered_set<string> supported_options = { "-h", "-c", "-t", "-m" };
const unordered_set<string> supported_methods = { "BM", "KMP", "FA", "BF" };

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);

    vector<string> args;
    unordered_set<string> options;
    parse_args_and_opts(argc, argv, args, options);

    for (const auto& opt : options) {
        if (supported_options.count(opt) == 0) {
            cerr << RED << "unsupported option(s)\n" << END;
            print_help(argv[0]);
            return -1;
        }
        else if (opt == "-h") {
            print_help(argv[0]);
            return 0;
        }
    }

    if (supported_methods.count(method) == 0) {
        cerr << RED << "unsupported method\n" << END;
        print_help(argv[0]);
        return -1;
    }

    if (args.size() != 3) {
        cerr << RED << "wrong arguments\n" << END;
        print_help(argv[0]);
        return -1;
    }

    auto patterns = split_partterns(args[1].c_str());
    if (patterns.size() == 0) {
        cerr << RED << "please provide at least one pattern\n" << END;
        return -1;
    }
    ifstream file{ args[2] };
    if (!file.is_open()) {
        cerr << RED << "error: cannot open file " << args[2] << '\n' << END;
        return -1;
    }

    // read the whole file into a string
    stringstream buffer;
    buffer << file.rdbuf();
    string text{ buffer.str() };

    vector<int> occurrences;
    vector<myStringAlgo::ac_result_t> ac_results;
    clock_t t0 = clock();
    if (patterns.size() == 1) {
        occurrences = search(patterns[0], text, method);
    }
    else {
        myStringAlgo::ACTrie ac(patterns);
        // the results only contain the longest suffix patterns
        // but prefix patterns still exist
        // ["wonderful job", "wonder", "job"] -> ["wonder", "wonderful job"]
        ac_results = ac.find(text.c_str());
    }
    clock_t t1 = clock();
    // in ms
    [[maybe_unused]] double time = (t1 - t0) / ((double) CLOCKS_PER_SEC / 1'000);

    if (options.count("-c") == 1) {
        if (patterns.size() == 1)
            cout << occurrences.size() << '\n';
        else
            cout << ac_results.size() << '\n';
    }
    else if (!occurrences.empty() || !ac_results.empty())  {
        vector<match_t> matches;
        if (occurrences.size() != 0) {
            matches.reserve(occurrences.size());
            int pat_len = patterns[0].size();
            for (auto pos : occurrences)
                matches.push_back({pos, pos + pat_len});
        }
        else {
            // eliminate prefix patterns
            // matches: |=====|===|=|=|===|==|=|=|
            //               ^   ^ ^ ^   ^  ^ ^ ^
            size_t prefix = 0;
            size_t sz = ac_results.size();
            for (size_t i = 1; i <= sz; i++) {
                while (i < sz && ac_results[i].match_begin == ac_results[prefix].match_begin)
                    i++;
                matches.push_back({ac_results[i-1].match_begin, ac_results[i-1].match_end});
                prefix = i;
            }
        }
        print_matches(text, matches);
    }

    if (options.count("-t") == 1)
        cout << time << " ms\n";
    return 0;
}
