#include "StringSearch.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <ctime>

using namespace std;

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define BLUE   "\033[0;34m"
#define END    "\033[0m"

void print_help(const char* arg0)
{
    cout << "Usage: " << arg0 << " [OPTION...] PATTERN FILE\n"
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

void print_matches(const string& text, const string& pattern,
                   const vector<int>& occurrences)
{
    const int M = occurrences.size();
    const int T = text.size();
    const int P = pattern.size();
    int i = 0, k = 0, line = 1;
    int start_of_line = 0;
    while (k < M && i < T) {
        if (i == occurrences[k]) { // print the whole line
            // follow color scheme of grep
            cout << GREEN << line << END << BLUE << ':' << END;
            for (int j = start_of_line; j != i; ++j)
                cout << text[j];
            // add i < T in case of noeol
            while (i < T && text[i] != '\n') {
                if (i == occurrences[k]) {
                    cout << RED; // highlight matches
                    for (int t = 0; t < P; ++t)
                        cout << text[i++];
                    cout << END;
                    ++k;
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

const unordered_set<string> supported_options = { "-h", "-c", "-t", "-m" };
const unordered_set<string> supported_methods = { "BM", "KMP", "FA", "BF" };

#define UNSUPPORTED_OPTION -2
#define UNSUPPORTED_METHOD -1
#define WRONG_ARGUMENTS 1
#define OPEN_FILE_ERROR 2

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
            return UNSUPPORTED_OPTION;
        }
        else if (opt == "-h") {
            print_help(argv[0]);
            return 0;
        }
    }

    if (supported_methods.count(method) == 0) {
        cerr << RED << "unsupported method\n" << END;
        print_help(argv[0]);
        return UNSUPPORTED_METHOD;
    }

    if (args.size() != 3) {
        cerr << RED << "wrong arguments\n" << END;
        print_help(argv[0]);
        return WRONG_ARGUMENTS;
    }

    string pattern { args[1] };
    ifstream file{ args[2] };
    if (!file.is_open()) {
        cerr << RED << "error: cannot open file " << args[2] << '\n' << END;
        return OPEN_FILE_ERROR;
    }

    // read the whole file into a string
    stringstream buffer;
    buffer << file.rdbuf();
    string text{ buffer.str() };

    [[maybe_unused]] double time;
    clock_t t0 = clock();
    vector<int> occurrences = search(pattern, text, method);
    clock_t t1 = clock();
    // in ms
    time = (t1 - t0) / ((double) CLOCKS_PER_SEC / 1'000);

    if (options.count("-c") == 1)
        cout << occurrences.size() << '\n';
    else
        print_matches(text, pattern, occurrences);

    if (options.count("-t") == 1)
        cout << time << " ms\n";
    return 0;
}
