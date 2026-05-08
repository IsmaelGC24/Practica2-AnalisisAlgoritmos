#include "knapsack.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity) {
    int n = (int)items.size();

    // Convertir a pesos y valores enteros según especificación:
    // w_i = redondeo(totalCharges)   [unidades de ancho de banda]
    // v_i = redondeo(monthlyCharges * 10) [centavos]
    std::vector<int> w(n), v(n);
    for (int i = 0; i < n; i++) {
        w[i] = (int)std::round(items[i].totalCharges);
        v[i] = (int)std::round(items[i].monthlyCharges * 10.0);
    }

    // dp[i][c] = valor máximo usando los primeros i elementos con capacidad c
    // Dimensiones: (n+1) x (capacidad+1)
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    // Llenar la tabla de programación dinámica
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= capacity; c++) {
            dp[i][c] = dp[i - 1][c]; // No tomar el elemento i
            if (w[i - 1] <= c) {     // Verificar si cabe el elemento i
                int take = dp[i - 1][c - w[i - 1]] + v[i - 1]; // Valor si se toma
                if (take > dp[i][c]) dp[i][c] = take;          // Quedarse con el mejor
            }
        }
    }

    // Retroceder para recuperar los elementos seleccionados
    std::vector<int> selected;
    int c = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i][c] != dp[i - 1][c]) {  // El elemento i fue tomado
            selected.push_back(i - 1);   // Índice en base 0 en el arreglo original
            c -= w[i - 1];               // Reducir la capacidad restante
        }
    }
    std::reverse(selected.begin(), selected.end());  // Ordenar en orden original

    return {dp[n][capacity], selected};  // Devolver valor máximo y elementos seleccionados
}
