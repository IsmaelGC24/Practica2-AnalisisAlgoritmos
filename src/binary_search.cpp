#include "binary_search.hpp"

// El arreglo se ordena de forma descendente por antiguedad (tenure)
// Busqueda binaria estandar para una coincidencia exacta (antiguedad == k).
// Teoria para este cpp
// En arreglo descendente: si arr[mid].tenure > k, el objetivo esta a la DERECHA (indices mas altos)
//                        si arr[mid].tenure < k, el objetivo esta a la IZQUIERDA (indices mas bajos)
//                        si arr[mid].tenure == k, encontrado — devuelve mid.
// Parametros: arr (vector de ServiceRequest), left (indice izquierdo), right (indice derecho), k (valor a buscar)
int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k) {
    if (left > right) return -1;          // Caso base: no encontrado

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure == k) {
        return mid;                        // Encontrado
    } else if (arr[mid].tenure > k) {
        // El objetivo es mas pequeno -> buscar en la mitad derecha (valores de antiguedad mas bajos)
        return binarySearchExact(arr, mid + 1, right, k);
    } else {
        // El objetivo es mas grande -> buscar en la mitad izquierda (valores de antiguedad mas altos)
        return binarySearchExact(arr, left, mid - 1, k);
    }
}
