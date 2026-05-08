#pragma once
#include <vector>

struct Edge {
    int    u, v;          // Vertices que conecta la arista
    double weight;        // Peso/costo de la arista
    bool operator<(const Edge& other) const { return weight < other.weight; }  // Para ordenar por peso
};

// Union-Find con union por rango y compresion de camino
struct UnionFind {
    std::vector<int> parent;   // Vector de padres
    std::vector<int> rank_;    // Vector de rangos (para union por rango)
    explicit UnionFind(int n);
    int  find(int x);          // Encuentra la raiz del conjunto de x (con compresion)
    bool unite(int x, int y);  // Une los conjuntos de x e y, retorna true si estaban en diferentes conjuntos
};

// Grafo no dirigido ponderado
struct Graph {
    int               numNodes;   // Numero de nodos en el grafo
    std::vector<Edge> edges;      // Lista de aristas
    void addEdge(int u, int v, double w);  // Agrega una arista entre u y v con peso w
};

// Construye el grafo de telecomunicaciones deterministico a partir de los promedios por grupo.
// groupAvg[k] = promedio de MonthlyCharges del grupo k (k = indice_del_registro mod 20).
// Parametros: groupAvg (vector de promedios por grupo)
Graph buildTelecomGraph(const std::vector<double>& groupAvg);
