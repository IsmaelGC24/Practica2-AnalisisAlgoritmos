#include "kruskal.hpp"
#include <algorithm>

// Parametros: g (referencia al grafo)
std::vector<Edge> kruskal(Graph& g) {
    // Kruskal es un algoritmo codicioso que construye el Arbol de Expansion Minima (MST).
    // Pretende conectar todos los nodos del grafo con el menor costo total posible, sin formar ciclos.
    // Esto se evidencia ordenando las aristas por peso y agregandolas solo si no crean ciclos.

    // Paso 1: Ordenar todas las aristas del grafo por peso ascendente.
    // Esto permite seleccionar las aristas mas ligeras primero, lo que es codicioso.
    std::sort(g.edges.begin(), g.edges.end());

    // Inicializar Union-Find para rastrear componentes conectadas.
    // Esto ayuda a detectar si agregar una arista crearia un ciclo.
    UnionFind uf(g.numNodes);

    // Vector para almacenar las aristas del MST.
    // Reservar espacio para n-1 aristas, ya que un arbol tiene n-1 aristas.
    std::vector<Edge> mst;
    mst.reserve(g.numNodes - 1);

    // Recorrer cada arista en orden de peso.
    for (const Edge& e : g.edges) {
        // Intentar unir los nodos u y v.
        // Si no estaban en la misma componente, se unen y se agrega la arista al MST.
        // Esto asegura que no se formen ciclos, ya que solo conecta componentes diferentes.
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            // Si ya tenemos n-1 aristas, el MST esta completo.
            // Detenerse temprano para optimizar.
            if ((int)mst.size() == g.numNodes - 1) break;
        }
        // Si ya estaban conectados, ignorar la arista para evitar ciclos.
    }

    // Devolver el MST, que conecta todos los nodos con costo minimo sin ciclos.
    return mst;
}
