#pragma once
#include "graph.hpp"
#include <vector>

// Returns the MST edges via Kruskal's algorithm.
// Uses Union-Find with union by rank + path compression.
std::vector<Edge> kruskal(Graph& g);
