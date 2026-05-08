# Práctica Final Integradora

**Análisis y Diseño de Algoritmos — EAFIT** 

Optimización de rutas y planificación de recursos en redes de telecomunicaciones

---

## Integrantes

David
Daniel
Ismael García Ceballos

---

## Descripción

Pipeline de tres módulos sobre el dataset *Telecom Customer Churn* (7 043 registros):

| Módulo | Paradigma | Algoritmo |
|--------|-----------|-----------|
| A | Divide y Vencerás | MergeSort + Búsqueda Binaria Recursiva |
| B | Codicioso | Kruskal con Union-Find (rango + compresión) |
| C | Programación Dinámica | Mochila 0-1 por tabulación + backtracking |

---

## Requisitos

- Compilador: `g++` con soporte C++17
- Sistema operativo: Linux, macOS o Windows (con MinGW/MSYS2)
- No se requieren dependencias externas

---

## Compilación

```bash
g++ -std=c++17 -O2 -o ada_pf src/main.cpp src/*.cpp
```

En Windows (PowerShell con MinGW):
```powershell
g++ -std=c++17 -O2 -o ada_pf.exe src/main.cpp src/parser.cpp src/mergesort.cpp src/binary_search.cpp src/graph.cpp src/kruskal.cpp src/knapsack.cpp
```

---

## Ejecución

```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

En Windows:
```powershell
.\ada_pf.exe data\WA_Fn-UseC_-Telco-Customer-Churn.csv
```

El programa genera automáticamente los cuatro archivos en `results/`.

---

## Valores de referencia esperados (Módulo A)

| Propiedad | Valor |
|-----------|-------|
| Total registros cargados | 7 043 |
| Registros con TotalCharges nulo | 11 |
| Registros con Churn = No | 5 174 |
| tenure máximo | 72 |
| tenure mínimo | 0 |
| MonthlyCharges promedio | 64.76 USD |

---

## Dataset

Fuente: [Kaggle — Telco Customer Churn](https://www.kaggle.com/datasets/blastchar/telco-customer-churn)  
Licencia: CC0 (dominio público)  
Archivo: `data/WA_Fn-UseC_-Telco-Customer-Churn.csv`

---