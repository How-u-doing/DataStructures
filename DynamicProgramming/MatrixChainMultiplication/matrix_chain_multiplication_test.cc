#include "matrix_chain_multiplication.h"

using namespace std;

int main()
{
    vector<vector<ull>> m;
    vector<vector<int>> s;
    // A_{0}  A_{1}  A_{2}  A_{3}  A_{4}  A_{5}
    // 30x35  35x15  15x5   5x10   10x20  20x25
    vector<int> p = {30, 35, 15, 5, 10, 20, 25};
    int n = p.size() - 1;
    optimal_matrix_chain_order(p, m, s);
    cout << "minimum number of multiplications needed is "
         << m[0][n-1] << '\n';
    print_optimal_parens(s, 0, n-1);
    cout << '\n';
    return 0;
}
