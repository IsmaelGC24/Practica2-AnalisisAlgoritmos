#pragma once
#include "parser.hpp"
#include <vector>

struct KnapsackResult {
    int                             totalValue;  // Valor óptimo (en centavos)
    std::vector<int>                selected;    // Índices dentro del vector original de elementos
};

// Mochila 0-1 mediante tabulación.
// items: vector de solicitudes de servicio (usa totalCharges como peso, monthlyCharges como valor)
// capacity: W en unidades de ancho de banda
KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity);
