#include "BloomFilter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
    try {
        size_t capacity = 1'000'000;
        float error_rate = 0.001f;
        if (argc < 3) {
            cout << "Usage: " << argv[0]
                 << " CORPUS_FILE QUERY_FILE [FILTER_CAPACITY=1'000'000] [ERROR_RATE=0.001]\n";
            return 0;
        }
        else if (argc > 4) {
            capacity = stoi(argv[3]);
            error_rate = stof(argv[4]);
        }
        else if (argc == 4) {
            capacity = stoi(argv[3]);
        }

        string corpus_file{ argv[1] };
        ifstream ifs{ corpus_file };
        if (!ifs.is_open()) {
            cerr << "cannot open file " << corpus_file << endl;
            return 1;
        }
        string query_file{ argv[2] };
        ifstream ifs2{ query_file };
        if (!ifs2.is_open()) {
            cerr << "cannot open file " << query_file << endl;
            return 1;
        }
        unordered_set<string> corpus;
        BloomFilter<string> filter(capacity, error_rate);
        for (string word; ifs >> word;) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            corpus.insert(word);
            filter.insert(word);
        }

#if 0
        // f.p. rate = 0 :)
        const char* queries[] = { "google", "youtube", "apple", "hey", "buzzword", "buzz", "word",
                                  "hot", "girl", "beautiful", "stunning", "amazing", "hi", "alison",
                                  "alice", "jacobi", "mark", "sophie", "marceau", "common", "words",
                                  "must", "have", "meet", "you", "by", "surprise", "i", "didn't",
                                  "realize", "that", "my", "life", "would", "change", "forever",
                                  "tell", "me", "that's", "true", "feelings", "that", "are", "new",
                                  "i", "feel", "something", "special", "about", "you", "dreams", "are",
                                  "my", "reality", "wannabe", "memoization", "dynamic", "programming" };
        const int query_num = sizeof queries / sizeof(const char*);
#endif

        unordered_set<string> queries;
        for (string word; ifs2 >> word;) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            queries.insert(word);
        }
        int false_positives = 0;
        for (const auto& query : queries) {
            if (!filter.contains(query) && corpus.count(query) == 1)
                ++false_positives;
        }

        float false_positive_rate = 1.0f * false_positives / queries.size();
        cout << "f.p. rate = " << false_positive_rate << '\t'
             << "error_rate = " << error_rate << '\n';
        cout << "filter.size() = " << filter.size() << '\t'
             << "corpus.size() = " << corpus.size() << '\n';
        cout << "filter.capacity() = " << filter.capacity() << '\n';
        cout << "filter.bytes_used() = " << filter.bytes_used() << '\n';
        cout << "filter.bitarray_size() = " << filter.bitarray_size() << '\n';
        cout << "filter.num_of_hash_func() = " << filter.num_of_hash_func() << '\n';
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
        return -1;
    }
    catch (...) {
        cout << "some unknown error happened :(" << endl;
        return -1;
    }
    return 0;
}
