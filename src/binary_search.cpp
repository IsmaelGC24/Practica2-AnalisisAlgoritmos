#include "binary_search.hpp"

// Array is sorted DESCENDING by tenure.
// Standard binary search for exact match (tenure == k).
// In a descending array: if arr[mid].tenure > k, the target is to the RIGHT (higher indices).
//                        if arr[mid].tenure < k, the target is to the LEFT  (lower indices).
//                        if arr[mid].tenure == k, found — return mid.

int binarySearchExact(const std::vector<ServiceRequest>& arr, int left, int right, int k) {
    if (left > right) return -1;          // base case: not found

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure == k) {
        return mid;                        // found
    } else if (arr[mid].tenure > k) {
        // target is smaller → look in right half (lower tenure values)
        return binarySearchExact(arr, mid + 1, right, k);
    } else {
        // target is larger → look in left half (higher tenure values)
        return binarySearchExact(arr, left, mid - 1, k);
    }
}
