#include "graph.hpp"
#include <cmath>
#include <stdexcept>

// Union-Find (Estructura de conjuntos disjuntos)

UnionFind::UnionFind(int n) : parent(n), rank_(n, 0) {
    for (int i = 0; i < n; i++) parent[i] = i;  // Inicialmente cada nodo es su propio padre
}

int UnionFind::find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]); // Compresion de camino
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;  // Ya estan en el mismo conjunto
    // Union por rango (optimizacion)
    if (rank_[rx] < rank_[ry]) std::swap(rx, ry);  // rx sera la raiz con mayor rango
    parent[ry] = rx;  // Unir ry debajo de rx
    if (rank_[rx] == rank_[ry]) rank_[rx]++;  // Incrementar rango si eran iguales
    return true;
}

// Grafo

void Graph::addEdge(int u, int v, double w) {
    edges.push_back({u, v, w});  // Agregar arista al vector de aristas
}

// Construccion deterministica del grafo
// Seccion 9: grupo i = indice_del_registro mod 20
// c(u,v) = piso( redondeo(Mu,2) + redondeo(Mv,2) )
// Parametros: groupAvg (vector de promedios por grupo)
Graph buildTelecomGraph(const std::vector<double>& groupAvg) {
    int N = (int)groupAvg.size(); // Debe ser 20 (grupos de CABA)
    Graph g;
    g.numNodes = N;

    // Recorrer todos los pares no ordenados de nodos (grafo completo)
    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            // Redondear cada promedio a 2 decimales antes de sumar
            double mu = std::round(groupAvg[u] * 100.0) / 100.0;
            double mv = std::round(groupAvg[v] * 100.0) / 100.0;
            double cost = std::floor(mu + mv);  // Costo = parte entera de la suma
            g.addEdge(u, v, cost);
        }
    }
    return g;
}