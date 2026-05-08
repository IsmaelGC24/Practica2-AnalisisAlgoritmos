#pragma once
#include "graph.hpp"
#include <vector>

// Devuelve las aristas del Árbol de Expansión Mínima (MST) mediante el algoritmo de Kruskal.
// Utiliza Union-Find con unión por rango + compresión de camino.
std::vector<Edge> kruskal(Graph& g);
