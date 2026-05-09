#pragma once
#include "parser.hpp"
#include <vector>

// Funcion de busqueda binaria recursiva en un arreglo ordenado por antiguedad DESCENDENTE.
// Devuelve el indice de UNA solicitud cuyo tenure sea exactamente igual a k.
// Si existen varios registros con tenure == k, retorna el indice de cualquiera de ellos.
// Devuelve -1 si no existe ningún elemento con tenure == k.
// Parametros: arr (vector de ServiceRequest), left (indice izquierdo), right (indice derecho), k (valor de consulta)
int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k);
