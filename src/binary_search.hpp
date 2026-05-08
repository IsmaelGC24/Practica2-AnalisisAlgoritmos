#pragma once
#include "parser.hpp"
#include <vector>

// Recursive binary search on an array sorted by tenure DESCENDING.
// Returns the index of ANY element whose tenure == k.
// Returns -1 if no such element exists.
int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k);
