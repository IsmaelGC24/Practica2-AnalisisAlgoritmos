#include "kruskal.hpp"
#include <algorithm>

std::vector<Edge> kruskal(Graph& g) {
    // 1. Sort edges by weight ascending
    std::sort(g.edges.begin(), g.edges.end());

    UnionFind uf(g.numNodes);
    std::vector<Edge> mst;
    mst.reserve(g.numNodes - 1);

    for (const Edge& e : g.edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            if ((int)mst.size() == g.numNodes - 1) break; // MST complete
        }
    }
    return mst;
}
