#include "knapsack.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

// Parametros: items (vector de ServiceRequest), capacity (capacidad de la mochila)
KnapsackResult knapsack01(const std::vector<ServiceRequest>& items, int capacity) {
    // La mochila 0-1 significa que cada item se toma completo o no se toma, sin fracciones.
    // Aqui se analiza la capacidad de la mochila y los pesos/valores de los items para decidir si llevar o no.
    // El enfoque es Programacion Dinamica por tabulacion: llena una tabla con subproblemas para evitar recalculos.
    // No es codicioso con paso greedy (no elige el mejor item primero), ni solo backtracking (es eficiente con tabla).

    int n = (int)items.size();

    // Convertir datos a enteros: pesos de totalCharges, valores de monthlyCharges.
    // Esto simplifica los calculos.
    std::vector<int> w(n), v(n);
    for (int i = 0; i < n; i++) {
        w[i] = (int)std::round(items[i].totalCharges);
        v[i] = (int)std::round(items[i].monthlyCharges * 10.0);
    }

    // Tabla dp donde dp[i][c] es el valor maximo con primeros i items y capacidad c.
    // Dimensiones: (n+1) x (capacidad+1), inicializada en 0.
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    // Llenar la tabla: para cada item y cada capacidad, decidir si tomar o no el item.
    // Esto evidencia el enfoque de PD, resolviendo subproblemas de menor a mayor.
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= capacity; c++) {
            // Opcion 1: no tomar el item i, usar valor de dp[i-1][c].
            dp[i][c] = dp[i - 1][c];
            // Opcion 2: si cabe (w[i-1] <= c), calcular valor si se toma.
            if (w[i - 1] <= c) {
                int take = dp[i - 1][c - w[i - 1]] + v[i - 1];
                // Elegir el maximo entre no tomar y tomar.
                if (take > dp[i][c]) dp[i][c] = take;
            }
        }
    }

    // Retroceder en la tabla para encontrar que items se seleccionaron.
    // Esto es backtracking sobre la tabla llena, no el algoritmo principal.
    std::vector<int> selected;
    int c = capacity;
    for (int i = n; i >= 1; i--) {
        // Si dp[i][c] > dp[i-1][c], el item i fue tomado.
        if (dp[i][c] != dp[i - 1][c]) {
            selected.push_back(i - 1);   // Agregar indice del item.
            c -= w[i - 1];               // Reducir capacidad restante.
        }
    }
    // Ordenar los indices en orden original.
    std::reverse(selected.begin(), selected.end());

    // Devolver el resultado: valor maximo y lista de items seleccionados.
    return {dp[n][capacity], selected};
}
