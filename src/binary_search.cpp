#include "binary_search.hpp"

// El arreglo está ordenado DESCENDENTEMENTE por antigüedad.
// Búsqueda binaria estándar para una coincidencia exacta (antigüedad == k).
// En un arreglo descendente: si arr[mid].tenure > k, el objetivo está a la DERECHA (índices más altos).
//                        si arr[mid].tenure < k, el objetivo está a la IZQUIERDA (índices más bajos).
//                        si arr[mid].tenure == k, encontrado — devuelve mid.

int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k) {
    if (left > right) return -1;          // Caso base: no encontrado

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure == k) {
        return mid;                        // Encontrado
    } else if (arr[mid].tenure > k) {
        // El objetivo es más pequeño → buscar en la mitad derecha (valores de antigüedad más bajos)
        return binarySearchExact(arr, mid + 1, right, k);
    } else {
        // El objetivo es más grande → buscar en la mitad izquierda (valores de antigüedad más altos)
        return binarySearchExact(arr, left, mid - 1, k);
    }
}
