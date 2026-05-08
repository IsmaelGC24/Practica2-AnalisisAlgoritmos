#pragma once
#include <vector>

struct Edge {
    int    u, v;
    double weight;
    bool operator<(const Edge& other) const { return weight < other.weight; }
};

// Union-Find with union by rank and path compression
struct UnionFind {
    std::vector<int> parent, rank_;
    explicit UnionFind(int n);
    int  find(int x);
    bool unite(int x, int y); // returns true if they were in different sets
};

// Weighted undirected graph
struct Graph {
    int               numNodes;
    std::vector<Edge> edges;
    void addEdge(int u, int v, double w);
};

// Builds the deterministic telecom graph from group averages.
// groupAvg[k] = average MonthlyCharges of group k (k = record_index mod 20).
Graph buildTelecomGraph(const std::vector<double>& groupAvg);
