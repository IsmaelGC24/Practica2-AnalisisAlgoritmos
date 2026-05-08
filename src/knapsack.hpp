#pragma once
#include "parser.hpp"
#include <vector>

struct KnapsackResult {
    int                             totalValue;  // Valor optimo (en centavos)
    std::vector<int>                selected;    // Indices dentro del vector original de elementos
};

// Mochila 0-1 mediante tabulacion.
// items: vector de solicitudes de servicio (usa totalCharges como peso, monthlyCharges como valor)
// capacity: W en unidades de ancho de banda
// Parametros: items (vector de ServiceRequest), capacity (capacidad de la mochila)
KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity);
