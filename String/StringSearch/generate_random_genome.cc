#include <random>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    string output_file;
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " SIZE (in MiB) [OUTPUT_FILE]\n";
        return 0;
    }
    if (argc == 2)
        output_file = "tmp.txt";
    else
        output_file = argv[2];

    const string sz = argv[1];
    int size;
    try {
       size = stoi(sz);
    }
    catch (...) {
        cerr << "stoi(" << argv[1] << ")\n";
        return -1;
    }

    if (size > 500) {
        cout << "maximum size allowed is 500 MiB\n";
        return 0;
    }
    size *= (1 << 20); // in MiB

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 3);

    string genome(size, '\0');

    // Ideally, each position has one fourth of possibility of being
    // A, C, G, and T. This implies if we randomly input 10 characters
    // for search in the DNA sequence of length 4^10 (1 MiB), then we
    // should hit one. For example, "AAAAAAAAAA" (10 As) should have 1
    // occurrence, so does 10 Cs/Gs/Ts. Any substring of 10 characters
    // should have 1 occurrence. You can imagine that each 10-character
    // substring is in a box (of size 10), and we number these boxes
    // from 1 to 4^10 (10 digits with radix of 4): "AAAAAAAAAA": 1
    // "CAAAAAAAAA": 2, "GAAAAAAAAA": 3, ..., etc.
    for (int i = 0, x; i < size; ++i) {
        x = distrib(gen);
        if      (x == 0) genome[i] = 'G';
        else if (x == 1) genome[i] = 'C';
        else if (x == 2) genome[i] = 'A';
        else if (x == 3) genome[i] = 'T';
    }

    ofstream ofs{ output_file };
    if (!ofs.is_open()) {
        cerr << "error: cannot open file " << output_file << '\n';
        return -2;
    }

    ofs << genome;
}
