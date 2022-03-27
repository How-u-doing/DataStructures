#include "BloomFilter.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    try {
        cout << "initializing a Bloom filter with capacity=13, error_rate=0.1...\n";
        BloomFilter<string> filter(13, 0.1f);
        cout << "initialization done\n\n";

        const char* corpus[] = { "hello", "world", "apple", "facebook", "google",
                                 "intel", "hp", "amazon", "microsoft", "netflix",
                                 "uber", "nvidia", "twitter", "adobe", "tesla" };
        const int N = sizeof corpus / sizeof(const char*);

        cout << "corpus:\n";
        for (int i = 0; i < N; ++i) {
            cout << corpus[i] << ' ';
            if ((i+1) % 5 == 0)
                cout << '\n';
        }
        cout << "\ninserting corpus words into the Bloom filter...\n";
        for (int i = 0; i < N; ++i) {
            int inserted = filter.insert(corpus[i]);
            if (inserted == 0)
                cout << corpus[i] << ": hashed positions taken\n";
            else if (inserted == -1)
                cout << "maximum size reached (filter is full) "
                     << "when inserting item '" << corpus[i] << "'\n";
        }
        cout << "insertion done\n\n";

        const char* queries[] = { "google", "youtube", "apple", "hey", "hello", "dell", "ibm",
                                  "meta", "facebook", "amazon", "orange", "twitter", "hp" };
        const int query_num = sizeof queries / sizeof(const char*);

        cout << "starting querying...\n";
        for (int i = 0; i < query_num; ++i) {
            cout << queries[i] << " : " << filter.contains(queries[i]) << '\n';
        }
        cout << "query done\n\n";
        cout << "filter.size() = " << filter.size() << '\n';
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
