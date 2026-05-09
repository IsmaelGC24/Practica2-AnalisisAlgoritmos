# Práctica Final Integradora

**Análisis y Diseño de Algoritmos — EAFIT** 

Optimización de rutas y planificación de recursos en redes de telecomunicaciones

---

## Integrantes

David Rodriguez Espinosa 

Daniel Mauricio Giraldo Moreno

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

En terminales Bash (Linux, macOS, Git Bash):
```bash
g++ -std=c++17 -O2 -o ada_pf src/*.cpp

En Windows (PowerShell / CMD con MinGW), se recomienda ser explícito con los archivos:
```powershell
g++ -std=c++17 -O2 -o ada_pf.exe src/main.cpp src/parser.cpp src/mergesort.cpp src/binary_search.cpp src/graph.cpp src/kruskal.cpp src/knapsack.cpp
```

---

## Ahora para poder realizar la Ejecución correctamente 
En terminales Bash ejecutar:
```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

### IMPORTANTE: INCONSISTENCIA MATEMÁTICA EN LA GUÍA Y JUSTIFICACIÓN DE INGENIERÍA (MÓDULO C): W=500

Se requiere hacer una aclaración crítica respecto a la capacidad de la mochila asignada para el Módulo C. La guía de evaluación establece textualmente las siguientes métricas:

- **Peso w_i:** TotalCharges redondeado al entero más cercano (unidades de ancho de banda requeridas).
- **Valor v_i:** MonthlyCharges multiplicado por 10 y redondeado (ingreso mensual en centavos).

A su vez, el requerimiento exige tomar como conjunto de entrada a las **50 solicitudes del arreglo ordenado con Churn=No**, en lugar de aquellas con menor cargo mensual.

**EL CONFLICTO MATEMÁTICO DE LOS DATOS:**

Al aplicar estrictamente las reglas de la guía sobre el dataset proporcionado, el orden de magnitud de los pesos (`TotalCharges`) para estos 50 clientes top se encuentra en los **MILES** cuando los valores de peso mínimo (w_i = TotalCharges) superan las 1000 unidades. Dado que los valores de la columna TotalCharges tienden a ser altos.

Por lo tanto, mantener la cota de capacidad de la mochila en **W = 500** hace que el problema sea poco practico o pueda resolver. La capacidad es insuficiente para admitir siquiera el ítem más ligero. 

**RESOLUCIÓN TÉCNICA:**
Si se conserva W = 500, el algoritmo devuelve un resultado TOTALMENTE NULO (0 ítems, 0 valor), lo cual impide que el grafo resuelva el problema y oculta el rendimiento deseado de la Programación Dinámica. 

Para habilitar la evaluación real del algoritmo y demostrar el correcto funcionamiento del *Backtracking* sobre la matriz, la capacidad fue **AJUSTADA a W = 50,000**. Esto permite que el sistema procese los datos reales y entregue una solución óptima verificable.


## Valores de referencia esperados (Módulo A) para verificar el procesamiento adecuado

| Propiedad | Valor |
|-----------|-------|
| Total registros cargados | 7 043 |
| Registros con TotalCharges nulo | 11 |
| Registros con Churn = No | 5 174 |
| tenure máximo | 72 |
| tenure mínimo | 0 |
---

## Dataset

Fuente: [Kaggle — Telco Customer Churn](https://www.kaggle.com/datasets/blastchar/telco-customer-churn)  
Licencia: CC0 (dominio público)  
Archivo: `data/WA_Fn-UseC_-Telco-Customer-Churn.csv`

---
