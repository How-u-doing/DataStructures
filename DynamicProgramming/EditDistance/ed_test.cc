#include "ed.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <source string> <target string>\n";
        return 0;
    }
    string S = argv[1];
    string T = argv[2];
    int d = edit_distance(S, T);
    cout << d << '\n';
    return 0;
}
