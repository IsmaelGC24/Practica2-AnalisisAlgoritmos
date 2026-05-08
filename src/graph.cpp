#include "graph.hpp"
#include <cmath>
#include <stdexcept>

// Union-Find

UnionFind::UnionFind(int n) : parent(n), rank_(n, 0) {
    for (int i = 0; i < n; i++) parent[i] = i;
}

int UnionFind::find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]); // path compression
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;
    // Union by rank
    if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
    parent[ry] = rx;
    if (rank_[rx] == rank_[ry]) rank_[rx]++;
    return true;
}

// Graph 

void Graph::addEdge(int u, int v, double w) {
    edges.push_back({u, v, w});
}

// Deterministic graph construction 
// Section 9: group i = record_index mod 20
// c(u,v) = floor( round(Mu,2) + round(Mv,2) )

Graph buildTelecomGraph(const std::vector<double>& groupAvg) {
    int N = (int)groupAvg.size(); // should be 20
    Graph g;
    g.numNodes = N;

    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            // Round each average to 2 decimal places before summing
            double mu = std::round(groupAvg[u] * 100.0) / 100.0;
            double mv = std::round(groupAvg[v] * 100.0) / 100.0;
            double cost = std::floor(mu + mv);
            g.addEdge(u, v, cost);
        }
    }
    return g;
}
