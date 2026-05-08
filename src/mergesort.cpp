#include "mergesort.hpp"
#include <vector>

static void merge(std::vector<ServiceRequest>& arr, int left, int mid, int right) {
    std::vector<ServiceRequest> leftPart(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<ServiceRequest> rightPart(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < (int)leftPart.size() && j < (int)rightPart.size()) {
        // El arreglo esta ordenado por antiguedad DESCENDENTE, asi que comparamos con >=
        if (leftPart[i].tenure >= rightPart[j].tenure) {
            arr[k++] = leftPart[i++];
        } else {
            arr[k++] = rightPart[j++];
        }
    }
    while (i < (int)leftPart.size()) arr[k++] = leftPart[i++];
    while (j < (int)rightPart.size()) arr[k++] = rightPart[j++];
}

// Parametros: arr (vector de ServiceRequest a ordenar), left (indice izquierdo), right (indice derecho)
void mergeSort(std::vector<ServiceRequest>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
