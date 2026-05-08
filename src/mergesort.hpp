#pragma once
#include "parser.hpp"
#include <vector>

// Ordena las solicitudes por antiguedad DESCENDENTE (estable, O(n log n) peor caso)
// Parametros: arr (vector de ServiceRequest a ordenar), left (indice izquierdo), right (indice derecho)
void mergeSort(std::vector<ServiceRequest>& arr, int left, int right);
