#pragma once
#include "parser.hpp"
#include <vector>

// Ordena las solicitudes por antigüedad DESCENDENTE (estable, O(n log n) peor caso)
void mergeSort(std::vector<ServiceRequest>& arr, int left, int right);
