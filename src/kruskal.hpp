#pragma once
#include "graph.hpp"
#include <vector>

// Devuelve las aristas del Arbol de Expansion Minima (MST) mediante el algoritmo de Kruskal.
// Utiliza Union-Find con union por rango + compresion de camino.
// Parametros: g (referencia al grafo)
std::vector<Edge> kruskal(Graph& g);
