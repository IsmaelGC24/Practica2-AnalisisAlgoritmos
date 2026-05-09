#include "binary_search.hpp"

// El arreglo se ordena de forma descendente por antiguedad (tenure)
// Busqueda binaria recursiva para encontrar la PRIMERA solicitud con tenure >= k.
// En arreglo descendente:
//   - si arr[mid].tenure >= k, el candidato puede estar en mid o a la izquierda.
//   - si arr[mid].tenure < k, la primer coincidencia debe estar a la derecha.
// Parametros: arr (vector de ServiceRequest), left (indice izquierdo), right (indice derecho), k (valor de consulta)
int binarySearchFirstGreaterOrEqual(const std::vector<ServiceRequest>& arr, int left, int right, int k) {
    if (left > right) return -1;          // Caso base: no encontrado

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure >= k) {
        // Puede haber un candidato mejor (primer elemento) a la izquierda.
        int leftResult = binarySearchFirstGreaterOrEqual(arr, left, mid - 1, k);
        return (leftResult != -1) ? leftResult : mid;
    } else {
        // En un arreglo descendente, si arr[mid].tenure < k entonces
        // todos los valores a la derecha también son menores.
        // La primera coincidencia debe estar a la izquierda.
        return binarySearchFirstGreaterOrEqual(arr, left, mid - 1, k);
    }
}
