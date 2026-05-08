#pragma once
#include "parser.hpp"
#include <vector>

// Búsqueda binaria recursiva en un arreglo ordenado por antigüedad DESCENDENTE.
// Devuelve el índice de CUALQUIER elemento cuya antigüedad sea == k.
// Devuelve -1 si no existe dicho elemento.
int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k);
