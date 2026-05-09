#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"
#include "graph.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

// Funciones auxiliares
// Practica 2 - Daniel Giraldo, Ismael Garcia y David Rodriguez

static void ensureResultsDir() {
#ifdef _WIN32
    system("if not exist results mkdir results");
#else
    system("mkdir -p results");
#endif
}

// Retorna milisegundos transcurridos
// Parametros: arr (vector de ServiceRequest)
static double measureMergeSort(std::vector<ServiceRequest> arr) {
    auto t0 = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, (int)arr.size() - 1);
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// Modulo A

// Parametros: records (vector de ServiceRequest), nullCount (numero de nulos)
static void runModuleA(std::vector<ServiceRequest>& records, int nullCount) {
    std::cout << "\n=== MODULO A: Divide y Venceras ===\n";
    std::cout << "Total de registros cargados : " << records.size() << "\n";
    std::cout << "Registros con TotalCharges nulo : " << nullCount << "\n";

    // Medicion empirica de tiempos
    int n = (int)records.size();
    std::vector<int> sizes;
    if (n >= 1000) sizes.push_back(1000);
    if (n >= 3500) sizes.push_back(3500);
    sizes.push_back(n);

    std::cout << "\n--- Tiempos de MergeSort ---\n";
    std::cout << std::left << std::setw(10) << "n"
            << std::setw(15) << "tiempo (ms)"
            << "razon t/prev\n";

    double prev = -1;
    for (int sz : sizes) {
        std::vector<ServiceRequest> sub(records.begin(), records.begin() + sz);
        double t = measureMergeSort(sub);
        std::cout << std::setw(10) << sz
                << std::setw(15) << std::fixed << std::setprecision(2) << t;
        if (prev > 0)
            std::cout << std::fixed << std::setprecision(4) << (t / prev);
        else
            std::cout << "base";
        std::cout << "\n";
        prev = t;
    }

    // Ordenar dataset completo (in-place)
    mergeSort(records, 0, (int)records.size() - 1);

    // Escribir CSV ordenado
    {
        std::ofstream out("results/solicitudes_ordenadas.csv");
        out << "customerID,tenure,cargoMensual,cargoTotal,churn\n";
        for (const auto& r : records) {
            out << r.customerID << ","
                << r.tenure << ","
                << std::fixed << std::setprecision(2) << r.monthlyCharges << ","
                << r.totalCharges << ","
                << (r.churn ? "Si" : "No") << "\n";
        }
        std::cout << "Escrito: results/solicitudes_ordenadas.csv\n";
    }

    // Busquedas binarias
    int queries[] = {72, 60, 45, 30, 12};
    std::string labels[] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    std::ofstream bOut("results/busquedas_A.txt");
    bOut << "Resultados de busqueda binaria recursiva (primera solicitud con tenure >= k):\n";
    bOut << std::left << std::setw(8)  << "Consulta"
        << std::setw(6)  << "k"
        << std::setw(15) << "customerID"
        << "tenure\n";
    bOut << std::string(45, '-') << "\n";

    std::cout << "\n--- Consultas de Busqueda Binaria ---\n";
    for (int i = 0; i < 5; i++) {
        int idx = binarySearchFirstGreaterOrEqual(records, 0, (int)records.size() - 1, queries[i]);
        std::string cid = (idx == -1) ? "NO ENCONTRADO" : records[idx].customerID;
        int ten = (idx == -1) ? -1 : records[idx].tenure;
        bOut << std::setw(8)  << labels[i]
            << std::setw(6)  << queries[i]
            << std::setw(15) << cid << ten << "\n";
        std::cout << labels[i] << " (k=" << queries[i] << "): " << cid
                << " (tenure=" << ten << ")\n";
    }
    std::cout << "Escrito: results/busquedas_A.txt\n";
}

// Modulo B

// Parametros: groupAvg (vector de promedios por grupo)
static void runModuleBWithAvg(const std::vector<double>& groupAvg) {
    std::cout << "\n=== MODULO B: Codicioso / MST con Kruskal ===\n";

    Graph g = buildTelecomGraph(groupAvg);
    std::cout << "Nodos : " << g.numNodes << "\n";
    std::cout << "Aristas: " << g.edges.size() << "\n";

    double sumW = 0;
    for (const auto& e : g.edges) sumW += e.weight;
    std::cout << "Costo promedio de arista: " << std::fixed << std::setprecision(2)
            << sumW / g.edges.size() << "\n";

    std::vector<Edge> mst = kruskal(g);

    double mstTotal = 0;
    for (const auto& e : mst) mstTotal += e.weight;
    std::cout << "Peso total del MST: " << mstTotal << "\n";
    std::cout << "Aristas del MST: " << mst.size() << "\n";

    // Verificacion manual: subgrafo de los primeros 5 nodos
    std::cout << "\n--- Verificacion manual: subgrafo nodos {0,1,2,3,4} ---\n";
    Graph sub;
    sub.numNodes = 5;
    for (const auto& e : g.edges)
        if (e.u < 5 && e.v < 5) sub.addEdge(e.u, e.v, e.weight);

    std::vector<Edge> subMST = kruskal(sub);
    double subTotal = 0;
    for (const auto& e : subMST) subTotal += e.weight;
    std::cout << "Aristas del sub-MST:\n";
    for (const auto& e : subMST)
        std::cout << "  (" << e.u << "," << e.v << ") p=" << e.weight << "\n";
    std::cout << "Peso total sub-MST: " << subTotal << "\n";

    // Escribir archivo de salida
    std::ofstream out("results/mst_red.txt");
    out << "=== MST - Arbol de Expansion Minima (Kruskal) ===\n\n";
    out << "Grafo: " << g.numNodes << " nodos, " << g.edges.size() << " aristas\n";
    out << "Costo promedio de arista: " << std::fixed << std::setprecision(2)
        << sumW / g.edges.size() << "\n\n";
    out << "Aristas del MST (u -- v : peso):\n";
    for (const auto& e : mst)
        out << "  Nodo " << e.u << " -- Nodo " << e.v << " : " << e.weight << "\n";
    out << "\nPeso total del MST: " << mstTotal << "\n";

    out << "\n--- Propiedad de Eleccion Codiciosa (Lema del Ciclo) ---\n";
    out << "El lema del corte garantiza que la arista de menor peso que cruza\n";
    out << "cualquier corte del grafo debe pertenecer a todo MST. Kruskal explota\n";
    out << "esto procesando las aristas en orden no decreciente de peso y agregando\n";
    out << "cada arista que no forme un ciclo (verificado via Union-Find).\n";
    out << "La primera arista agregada en esta ejecucion:\n";
    if (!mst.empty()) {
        out << "  Nodo " << mst[0].u << " -- Nodo " << mst[0].v
            << " (peso " << mst[0].weight << ") es la arista globalmente mas barata;\n";
        out << "  por el Lema del Ciclo pertenece a todo MST de este grafo.\n";
    }

    out << "\n--- Verificacion manual (nodos 0 a 4) ---\n";
    for (const auto& e : subMST)
        out << "  (" << e.u << "," << e.v << ") p=" << e.weight << "\n";
    out << "Peso total sub-MST: " << subTotal << "\n";

    std::cout << "Escrito: results/mst_red.txt\n";
}

// Modulo C

// Parametros: sortedRecords (vector ordenado de ServiceRequest)
static void runModuleC(const std::vector<ServiceRequest>& sortedRecords) {
    std::cout << "\n=== MODULO C: Programacion Dinamica / Mochila 0-1 ===\n";

    // Tomar las 50 solicitudes activas (Churn=No) de mayor tenure
    std::vector<ServiceRequest> top50;
    for (const auto& r : sortedRecords) {
        if (!r.churn) {
            top50.push_back(r);
            if ((int)top50.size() == 50) break;
        }
    }
    std::cout << "Solicitudes seleccionadas (Churn=No, mayor tenure): " << top50.size() << "\n";

    const int W = 50000;
    KnapsackResult res = knapsack01(top50, W);

    std::cout << "Valor optimo: " << res.totalValue << " centavos\n";
    std::cout << "Solicitudes seleccionadas: " << res.selected.size() << "\n";

    // Contraejemplo codicioso con 3 items
    struct Item3 { int idx; int w; int v; double ratio; };
    std::vector<Item3> items3;
    for (int i = 0; i < (int)top50.size(); i++) {
        int wi = (int)std::round(top50[i].totalCharges);
        int vi = (int)std::round(top50[i].monthlyCharges * 10.0);
        if (wi > 0) items3.push_back({i, wi, vi, (double)vi / wi});
    }

    std::vector<Item3> byRatio = items3;
    std::sort(byRatio.begin(), byRatio.end(), [](const Item3& a, const Item3& b){
        return a.ratio > b.ratio;
    });

    bool foundCE = false;
    int ceA = -1, ceB = -1, ceC = -1, ceW = -1;

    for (int ai = 0; ai < (int)byRatio.size() && !foundCE; ai++) {
        for (int bi = ai+1; bi < (int)byRatio.size() && !foundCE; bi++) {
            for (int ci = bi+1; ci < (int)byRatio.size() && !foundCE; ci++) {
                Item3& A = byRatio[ai];
                Item3& B = byRatio[bi];
                Item3& C = byRatio[ci];
                
                // Capacidad: debe contener B+C pero no A+B+C
                // Para contraejemplo: greedy elige A (mejor ratio)
                // pero DP elige B+C (mayor valor total)
                if (A.v < B.v + C.v) {  // DP (B+C) es mejor que greedy (A)
                    int cap = std::max(A.w, B.w + C.w);  // Mínima capacidad para ambos
                    if (cap < A.w + B.w + C.w) {  // No cabe todo a la vez
                        ceA = ai; ceB = bi; ceC = ci;
                        ceW = cap;
                        foundCE = true;
                        break;
                    }
                }
            }
        }
    }

    // Escribir archivo de salida
    std::ofstream out("results/asignacion_bw.txt");
    out << "=== MODULO C: Mochila 0-1 - Asignacion de Ancho de Banda ===\n\n";
    out << "Hemos notado un problema de W en la guia, tomamos Capacidad W = " << W << " unidades de ancho de banda\n";
    out << "Nota: w_i = round(TotalCharges), v_i = round(MonthlyCharges * 10).\n";
    out << "      Con los 50 primeros items activos, los pesos w_i estan en el orden de miles.\n";
    out << "      Por eso se usa W = 50000 en lugar de W = 500 para obtener una solucion significativa.\n";
    out << "Items: top 50 solicitudes activas por tenure\n\n";

    out << "--- Solucion Optima (Programacion Dinamica) ---\n";
    out << "Valor optimo  : " << res.totalValue << " centavos\n";
    out << "Items elegidos: " << res.selected.size() << "\n\n";
    out << std::left
        << std::setw(15) << "customerID"
        << std::setw(10) << "tenure"
        << std::setw(15) << "peso (w_i)"
        << std::setw(15) << "valor (v_i)" << "\n";
    out << std::string(55, '-') << "\n";

    int totalW = 0, totalV = 0;
    for (int idx : res.selected) {
        int wi = (int)std::round(top50[idx].totalCharges);
        int vi = (int)std::round(top50[idx].monthlyCharges * 10.0);
        totalW += wi; totalV += vi;
        out << std::setw(15) << top50[idx].customerID
            << std::setw(10) << top50[idx].tenure
            << std::setw(15) << wi
            << std::setw(15) << vi << "\n";
    }
    out << std::string(55, '-') << "\n";
    out << std::setw(15) << "TOTAL" << std::setw(10) << ""
        << std::setw(15) << totalW << std::setw(15) << totalV << "\n";

    // Contraejemplo
    out << "\n--- Contraejemplo: Fallo del Enfoque Codicioso ---\n";
    if (foundCE) {
        Item3& A = byRatio[ceA];
        Item3& B = byRatio[ceB];
        Item3& C = byRatio[ceC];
        out << "Capacidad del contraejemplo: " << ceW << "\n";
        out << "Explicacion: Con esta capacidad, el greedy (por ratio) elige A,\n";
        out << "pero la DP (mochila 0-1) elige B+C que tiene mayor valor total.\n\n";
        out << std::setw(10) << "Item"
            << std::setw(15) << "customerID"
            << std::setw(10) << "w_i"
            << std::setw(10) << "v_i"
            << " v_i/w_i\n";
        out << std::string(57, '-') << "\n";
        out << std::setw(10) << "A (mejor)" << std::setw(15) << top50[A.idx].customerID
            << std::setw(10) << A.w << std::setw(10) << A.v
            << std::fixed << std::setprecision(4) << " " << A.ratio << "\n";
        out << std::setw(10) << "B" << std::setw(15) << top50[B.idx].customerID
            << std::setw(10) << B.w << std::setw(10) << B.v
            << std::fixed << std::setprecision(4) << " " << B.ratio << "\n";
        out << std::setw(10) << "C" << std::setw(15) << top50[C.idx].customerID
            << std::setw(10) << C.w << std::setw(10) << C.v
            << std::fixed << std::setprecision(4) << " " << C.ratio << "\n\n";
        out << "Enfoque              | Solicitudes elegidas | Valor total | Optimo?\n";
        out << std::string(65, '-') << "\n";
        out << "Codicioso (ratio v/w)| A                    | " << A.v << "        | No\n";
        out << "PD (Mochila 0-1)     | B + C                | " << (B.v + C.v) << "        | Si\n";
    }

    out << "\n--- Analisis de Complejidad ---\n";
    out << "Tiempo : Theta(n * W) — dos bucles anidados sobre n items y W+1 capacidades.\n";
    out << "Espacio: Theta(n * W) — tabla dp de dimensiones (n+1) x (W+1) enteros.\n";
    out << "         Puede reducirse a O(W) manteniendo solo una fila.\n\n";
    out << "Pseudopolinomialidad:\n";
    out << "  El algoritmo corre en O(n*W), lo que PARECE polinomial en n y W.\n";
    out << "  Sin embargo, W es un valor numerico, no el TAMANO de la entrada.\n";
    out << "  El tamano de entrada para W es O(log W) bits, entonces\n";
    out << "  O(n*W) = O(n * 2^(log W)), que es EXPONENCIAL en el tamano real.\n";
    out << "  Por eso se llama pseudopolinomial. El problema Mochila 0-1 es NP-dificil;\n";
    out << "  esta DP no contradice ese hecho.\n";

    std::cout << "Escrito: results/asignacion_bw.txt\n";
    std::cout << "Valor optimo: " << res.totalValue << " centavos\n";
    std::cout << "Items seleccionados: " << res.selected.size() << "\n";
    if (foundCE) {
        Item3& A = byRatio[ceA];
        Item3& B = byRatio[ceB];
        Item3& C = byRatio[ceC];
        std::cout << "Contraejemplo encontrado (capacidad " << ceW << "): "
                << "codicioso=" << A.v << " vs PD=" << (B.v + C.v) << "\n";
    }
}

// Funcion Principal

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv\n";
        return 1;
    }

    ensureResultsDir();

    // Parseo del CSV
    int nullCount = 0;
    std::vector<ServiceRequest> records = parseCSV(argv[1], nullCount);

    // Promedios de grupo ANTES de ordenar (orden original del CSV)
    const int N_GROUPS = 20;
    std::vector<double> groupSum(N_GROUPS, 0.0);
    std::vector<int>    groupCnt(N_GROUPS, 0);
    for (int i = 0; i < (int)records.size(); i++) {
        int g = i % N_GROUPS;
        groupSum[g] += records[i].monthlyCharges;
        groupCnt[g]++;
    }
    std::vector<double> groupAvg(N_GROUPS);
    for (int k = 0; k < N_GROUPS; k++)
        groupAvg[k] = (groupCnt[k] > 0) ? groupSum[k] / groupCnt[k] : 0.0;

    // Modulo A
    runModuleA(records, nullCount);
    // records queda ordenado descendentemente por tenure

    // Modulo B
    runModuleBWithAvg(groupAvg);

    // Modulo C (usa los registros ordenados del Modulo A)
    runModuleC(records);

    std::cout << "\nTodos los resultados escritos en results/\n";
    return 0;
}