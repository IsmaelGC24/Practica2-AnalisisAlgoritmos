#pragma once
#include "parser.hpp"
#include <vector>

// Sorts requests by tenure DESCENDING (stable, O(n log n) worst case)
void mergeSort(std::vector<ServiceRequest>& arr, int left, int right);
