#include "kruskal.hpp"
#include <algorithm>

std::vector<Edge> kruskal(Graph& g) {
    // 1. Ordenar las aristas por peso (costo)
    std::sort(g.edges.begin(), g.edges.end());

    UnionFind uf(g.numNodes);
    std::vector<Edge> mst;
    mst.reserve(g.numNodes - 1);

    for (const Edge& e : g.edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            if ((int)mst.size() == g.numNodes - 1) break; // MST completo (n-1 aristas)
        }
    }
    return mst;
}
