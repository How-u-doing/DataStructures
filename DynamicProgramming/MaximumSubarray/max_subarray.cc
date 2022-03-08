#include <iostream>
#include <vector>

typedef long long LL;

// Kadane's algorithm, runs in O(n) time.
// see also https://en.wikipedia.org/wiki/Maximum_subarray_problem
// Essentially, if we denote dp[i] the maximum sum of the subarray
// ended with index i, then we have
//               dp[i] = max(dp[i-1]+arr[i], arr[i])
// that is if dp[i-1] < 0, we set dp[i] the current element arr[i].
// I (arr[i]) need you (dp[i-1]) only if you contribute positively,
// otherwise I am cool all by myself :)
// In the last, we set max_sum = max(dp[0..n-1]).
LL max_subarray(const std::vector<int>& arr, size_t& lo, size_t& hi)
{
    LL tentative_sum = 0;
    LL max_sum = 0;
    size_t tentative_lo = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        tentative_sum += arr[i];
        if (tentative_sum < 0) {
            tentative_sum = 0; // start next round of test
            tentative_lo = i + 1;
        }
        if (tentative_sum > max_sum) {
            max_sum = tentative_sum;
            lo = tentative_lo;
            hi = i;
        }
    }
    return max_sum; // return 0 if no positive numbers
}

int main()
{
    std::vector<int> arr;
    int x;
    while (std::cin >> x) {
        arr.push_back(x);
    }

    size_t lo = 0, hi = 0;
    LL max_sum = max_subarray(arr, lo, hi);
    std::cout << "Maximum subarray is arr[" << lo << ".."
              << hi << "], with max sum = " << max_sum << '\n';
    return 0;
}
