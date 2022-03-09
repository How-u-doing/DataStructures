#include <iostream>
#include <vector>
#include <string>

typedef unsigned long long ULL;

/*
 * Naive recursive algorithm:
 *   fib(n):
 *     if n <= 2: f = 1
 *     else: f = fib(n-1) + fib(n-2)
 *     return f
 * Complexity: exponential
 * Explanation: T(n) = T(n-1) + T(n-2) + O(1)
 * Thus, T(n) >= 2T(n-2) = O(2^(n/2))
 *
 * Memoized (memo -ized) DP algorithm:
 *   memo = {}
 *   fib(n):
 *     if n in memo: return memo[n]
 *     if n <= 2: f = 1
 *     else: f = fib(n-1) + fib(n-2)
 *     memo[n] = f
 *     return f
 *
 * Or iteratively:
 *   fib = {}
 *   for k in range(1:n+1):
 *     if k <= 2: f = 1
 *     else: f = fib[k-1] + fib[k-2]
 *     fib[k] = f
 *   return fib[n]
 *
 * Complexity: O(n). Obviously, we call fib(n-1)...fib(1)
 * each exactly once, each call is constant time.
 *
 * See also <https://www.youtube.com/watch?v=OQ5jsbhAv_M>
 * by Erik Demaine, a truly respectable computer scientist.
 *
 * Also, an iterative solution is easy to develop:
 *   fib(n):
 *     a = 0, b = 1
 *     for _ in range(n):
 *       t = a
 *       a = b
 *       b = t + b
 *       # or equivalently:
 *       # a, b = b, a+b
 *     return a
 *
 * For a more advanced algorithm to compute the Fibnacci sequence
 * in O(log n) time, please see
 * https://en.wikipedia.org/wiki/Fibonacci_number#Matrix_form
 */
ULL fib(int n)
{
    static std::vector<ULL> memo(n+1, 0);
#if 1
    for (int i = 1; i <= n; ++i) {
        if (i <= 2) memo[i] = 1;
        else memo[i] = memo[i-1] + memo[i-2];
    }
    return memo[n];
#else
    if (memo[n] != 0) return memo[n];
    if (n == 0) return 0; // add this for completeness
    if (n <= 2) return 1;
    return memo[n] = fib(n-1) + fib(n-2);
#endif
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " number\n";
        return 0;
    }

    try {
        int n = std::stoi(std::string(argv[1]));
        // fib(93) = 12200160415121876738
        // fib(94) = 19740274219868223167 > 2^64 = 1.8446744e+19
        if (n > 93) {
            std::cout << "Maximum fib number can be computed is fib(93)\n"
                         "For larger numbers, please run"
                         " 'python3 fib.py number' instead\n";
            return 0;
        }
        if (n < 0) {
            std::cout << "Number cannot be negative\n";
            return 1;
        }
        std::cout << fib(n) << '\n';
    }
    catch (...) {
        std::cout << "Invalid number " << argv[1] << '\n';
        return 2;
    }
    return 0;
}
