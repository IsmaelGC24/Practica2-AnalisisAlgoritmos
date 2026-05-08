#pragma once
#include "parser.hpp"
#include <vector>

struct KnapsackResult {
    int                             totalValue;  // optimal value (centavos)
    std::vector<int>                selected;    // indices into the input items vector
};

// 0-1 Knapsack via tabulation.
// items: vector of service requests (uses totalCharges as weight, monthlyCharges as value)
// capacity: W in bandwidth units
KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity);
