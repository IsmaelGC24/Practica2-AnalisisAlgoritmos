#pragma once
#include "parser.hpp"
#include <vector>

// Funcion de busqueda binaria recursiva en un arreglo ordenado por antiguedad DESCENDENTE.
// Devuelve el indice de CUALQUIER elemento cuya antiguedad sea == k.
// Devuelve -1 si no existe dicho elemento.
// Parametros: arr (vector de ServiceRequest), left (indice izquierdo), right (indice derecho), k (valor a buscar)
int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k);
