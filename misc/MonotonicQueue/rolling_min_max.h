#pragma once

#include <algorithm>
#include <deque>
#include <functional>
#include <set>
#include <vector>
#include <cctype>
#include <cmath>

inline std::vector<int> rolling_max_large_window(const std::vector<int> &nums, int w) {
    std::vector<int> res;
    res.reserve(nums.size() - w + 1);
    // Use greater so that the greatest element is at s.begin(). Otherwise, we need to
    // dereference rbegin(), i.e., *--s.end(), which requires an extra `_Rb_tree_decrement()`
    // call (which has a fast path for the header case though).
    std::multiset<int, std::greater<int>> s(nums.begin(), nums.begin() + w);
    res.push_back(*s.begin());
    for (int i = w; i < (int)nums.size(); i++) {
        s.erase(s.find(nums[i - w]));
        s.insert(nums[i]);
        res.push_back(*s.begin());
    }
    return res;
}

inline std::vector<int> rolling_max_naive(const std::vector<int> &nums, int w) {
    // if (w > 64)
    //     return rolling_max_large_window(nums, w);
    std::vector<int> res;
    res.reserve(nums.size() - w + 1);
    res.push_back(*max_element(nums.begin(), nums.begin() + w));
    for (int i = w; i < (int)nums.size(); i++) {
        // leaving element is greatest
        if (nums[i - w] == res[res.size() - 1]) {
            res.push_back(*max_element(nums.begin() + i - w + 1, nums.begin() + i + 1));
        } else if (nums[i] < res[res.size() - 1]) {
            res.push_back(res[res.size() - 1]);
        } else {
            res.push_back(nums[i]);
        }
    }
    return res;
}

inline std::vector<int> rolling_max(const std::vector<int> &nums, int w) {
    std::vector<int> res;
    res.reserve(nums.size() - w + 1);
    std::deque<int> dq;  // maintains a monotonically decreasing sequence

    for (int i = 0; i < (int)nums.size(); i++) {
        // remove the maximum element if it's outside the current window
        if (!dq.empty() && dq.front() <= i - w)
            dq.pop_front();

        // add the current index to the deque while maintaining the monotonicity
        // e.g., add 2 into dq=[4, 2, 1]: pop 1, 2, push back the new 2
        while (!dq.empty() && nums[i] >= nums[dq.back()])
            dq.pop_back();
        dq.push_back(i);

        if (i >= w - 1)
            res.push_back(nums[dq.front()]);
    }
    return res;
}

typedef int64_t idx_t;

template <typename Compare = std::greater<double>>
void rolling_min_max_nonulls(const double *data, double *res, idx_t len, int w,
                             Compare cmp = Compare{}) {
    // dq maintains a monotonically increasing/decreasing sequence for rolling min/max
    std::deque<idx_t> dq;

    for (idx_t i = 0; i < len; i++) {
        // remove the extremum if it's outside the current window
        if (!dq.empty() && dq.front() <= i - w)
            dq.pop_front();

        // add the current index to the deque while maintaining the monotonicity
        // e.g., add 2 into dq=[4, 2, 1]: pop 1, 2, push back the new 2
        while (!dq.empty() && !cmp(data[dq.back()], data[i]))
            dq.pop_back();
        dq.push_back(i);

        if (i >= w - 1)
            res[i] = data[dq.front()];
        else
            res[i] = NAN;
    }
}
