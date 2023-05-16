#include "rolling_rank.h"

#include <iostream>
#include <cstdlib>

using namespace myRankingAlgo;
using namespace std;

void get_rank_naive(double *res, RankMethod method, bool asc, const double *arr,
                    int arr_size, int window) {
    for (int i = 0; i < arr_size; ++i) {
        if (i < window - 1) {
            res[i] = -1;  // or NAN
        } else {
            res[i] = rank_naive(arr, method, asc, i, i - window + 1, i + 1);
        }
    }
}

void fill_array(double *arr, int size, double min, double max) {
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        arr[i] = ((double)rand() / RAND_MAX) * (max - min) + min;
    }
}

int unequal_size(const double *res1, const double *res2, int size) {
    int unequal = 0;
    for (int i = 0; i < size; ++i) {
        if (res1[i] != res2[i]) {
            ++unequal;
        }
    }
    return unequal;
}

constexpr RankMethod method = RankMethod::Average;
constexpr bool asc = true;

void test() {
    double arr[]{0.7, 0.8, 0.2, 0.15, 0.1, 0.2,  0.3, 0.25, 0.3,
                 0.2, 0.3, 0.7, 0.3,  0.5, 0.45, 0.3, 0.2};
    const int arr_size = sizeof arr / sizeof *arr;
    const int window = 5;

    double res1[arr_size]{};
    get_rank_naive(res1, method, asc, arr, arr_size, window);
    for (auto x : res1) {
        cout << x << " ";
    }
    cout << '\n';

    double res2[arr_size]{};
    rolling_rank<double, asc>(res2, method, arr, arr_size, window);
    for (auto x : res2) {
        cout << x << " ";
    }
    cout << '\n';
}

constexpr int arr_size = 1024 * 1024;
double arr[arr_size];
double res_naive[arr_size];
double res_tree[arr_size];

void run_benchmark(int window) {
    const double min = 100'000;
    const double max = min + arr_size * 0.1;
    fill_array(arr, arr_size, min, max);

    clock_t t1, t2;

    t1 = clock();
    get_rank_naive(res_naive, method, asc, arr, arr_size, window);
    t2 = clock();
    const double native_time = (t2 - t1) / (double)CLOCKS_PER_SEC;

    t1 = clock();
    rolling_rank<double, asc>(res_tree, method, arr, arr_size, window);
    t2 = clock();
    const double rank_tree_time = (t2 - t1) / (double)CLOCKS_PER_SEC;

    const int unequal = unequal_size(res_naive, res_tree, arr_size);
    cout << "unequal size = " << unequal << '\n'
         << "native time = " << native_time << '\n'
         << "rank tree time = " << rank_tree_time << '\n';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " window\n";
        return EXIT_FAILURE;
    }
    // no error handling for simplicity
    const int window = atoi(argv[1]);

    test();
    run_benchmark(window);

    return EXIT_SUCCESS;
}
