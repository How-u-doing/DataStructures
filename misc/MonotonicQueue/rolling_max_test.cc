#include "rolling_min_max.h"

#include <iostream>
#include <ctime>

using namespace std;

int main() {
#if 1
    constexpr int N = 100'000;
    constexpr int w = 500'000;
    vector<int> nums(N * 10);
    for (size_t i = 0; i < nums.size(); i++) {
        nums[i] = N - i / 10;
    }
    auto t1 = clock();
    auto res = rolling_max(nums, w);
    auto t2 = clock();
#else
    vector<double> data{1, 3, -1, -3, 5, 3, 6, 7};
    constexpr int w = 3;
    vector<double> res(data.size());
    auto t1 = clock();
    rolling_min_max_nonulls(data.data(), res.data(), data.size(), w /*, std::less{} */);
    auto t2 = clock();
    for (auto x : res)
        cout << x << " ";
#endif
    cout << "\n" << res.size() << " " << res[0] << " " << res[res.size() - 1];
    cout << "\n " << (t2 - t1) / (double)CLOCKS_PER_SEC << '\n';
}
