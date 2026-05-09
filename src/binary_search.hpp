#pragma once
#include "parser.hpp"
#include <vector>

// Funcion de busqueda binaria recursiva en un arreglo ordenado por antiguedad DESCENDENTE.
// Devuelve el indice de la PRIMERA solicitud cuyo tenure sea mayor o igual a k.
// Devuelve -1 si no existe ningun elemento con tenure >= k.
// Parametros: arr (vector de ServiceRequest), left (indice izquierdo), right (indice derecho), k (valor de consulta)
int binarySearchFirstGreaterOrEqual(const std::vector<ServiceRequest>& arr, int left, int right, int k);
