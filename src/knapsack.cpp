#include "knapsack.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity) {
    int n = (int)items.size();

    // Convert to integer weights and values per spec:
    // w_i = round(totalCharges)   [bandwidth units]
    // v_i = round(monthlyCharges * 10) [centavos]
    std::vector<int> w(n), v(n);
    for (int i = 0; i < n; i++) {
        w[i] = (int)std::round(items[i].totalCharges);
        v[i] = (int)std::round(items[i].monthlyCharges * 10.0);
    }

    // dp[i][c] = max value using first i items with capacity c
    // Dimensions: (n+1) x (W+1)
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= capacity; c++) {
            dp[i][c] = dp[i - 1][c]; // don't take item i
            if (w[i - 1] <= c) {
                int take = dp[i - 1][c - w[i - 1]] + v[i - 1];
                if (take > dp[i][c]) dp[i][c] = take;
            }
        }
    }

    // Backtracking to recover selected items
    std::vector<int> selected;
    int c = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i][c] != dp[i - 1][c]) {
            selected.push_back(i - 1); // 0-based index into items
            c -= w[i - 1];
        }
    }
    std::reverse(selected.begin(), selected.end());

    return {dp[n][capacity], selected};
}
