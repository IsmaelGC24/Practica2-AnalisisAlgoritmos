# Práctica Final Integradora: Análisis y Diseño de Algoritmos
## Optimización de Rutas y Planificación de Recursos en Redes de Telecomunicaciones

---

## Portada

**Institución:** EAFIT — Escuela de Administración, Finanzas e Ingeniería Tecnológica

**Curso:** Análisis y Diseño de Algoritmos

**Título:** Integración de Paradigmas Algorítmicos en Datasets Reales

**Integrantes:**
- David Rodriguez
- Daniel  Giraldo
- Ismael García Ceballos

**Fecha:** Mayo de 2026

---

## 1. Introducción

### 1.1 Descripción del Problema

Esta práctica integra tres paradigmas fundamentales del diseño algorítmico mediante el análisis del dataset *Telecom Customer Churn* de Kaggle, que contiene 7.043 registros de clientes de telecomunicaciones con atributos como antigüedad (tenure), cargos mensuales/totales y estado de deserción (churn).

El pipeline implementado consta de tres módulos independientes pero complementarios:

1. **Módulo A (Divide y Vencerás):** Ordena registros por antigüedad mediante MergeSort y ejecuta búsquedas exactas con búsqueda binaria recursiva.
2. **Módulo B (Codicioso):** Construye un Árbol de Expansión Mínima mediante Kruskal con Union-Find, modelando una red de telecomunicaciones.
3. **Módulo C (Programación Dinámica):** Resuelve la Mochila 0-1 como problema de asignación de ancho de banda, comparándose contra el enfoque codicioso.

### 1.2 Contexto del Dataset

**Características del dataset:**
- **Tamaño:** 7.043 registros de clientes
- **Dimensiones:** 21 atributos (ID, características demográficas, servicios, cargos, churn)
- **Atributos relevantes:** 
  - `tenure`: Antigüedad en meses (rango: 0–72)
  - `monthlyCharges`: Cargo mensual en USD
  - `totalCharges`: Cargo total acumulado (con **11 valores nulos**)
  - `Churn`: Indicador binario de deserción

### 1.3 Justificación de Paradigmas

| Módulo | Problema | Paradigma | Razón |
|--------|----------|-----------|-------|
| A | Búsqueda en dataset no ordenado | Divide y Vencerás | MergeSort (O(n log n)) es óptimo para ordenamiento general; la búsqueda binaria explota la estructura ordenada para O(log n) por consulta |
| B | Minimizar costo de conectividad en red | Codicioso | Kruskal garantiza MST óptimo; la selección local (arista de menor peso sin ciclos) conduce a óptimo global |
| C | Maximizar valor bajo restricción de capacidad | Programación Dinámica | La mochila 0-1 admite solape de subproblemas; DP evita recalculos (pseudopolinomial); codicioso falla |

---

## 2. Descripción del Dataset

### 2.1 Estadísticas del CSV Cargado

```
Total de registros: 7.043
Registros procesados: 7.032
Registros con TotalCharges nulo: 11
Tasa de valores nulos: 0.156%

Atributo        | Mín      | Máx      | Promedio
-----------------------------------------------------
tenure          | 0 meses  | 72 meses | 32.4 meses
monthlyCharges  | $18.30   | $118.75  | $64.80
totalCharges    | $0       | $8.672   | $2.145
```

### 2.2 Manejo de Valores Nulos

Los 11 registros con `TotalCharges = nulo` corresponden a clientes con `tenure = 0` (nuevos) que aún no tienen cargos acumulados. El parser implementado en `parser.cpp`:

1. Detecta campos vacíos o solo espacios en la columna `TotalCharges`
2. Asigna valor `0.0` a estos registros
3. Incrementa un contador `nullCount` para auditoría
4. Continúa procesando sin fallar

**Estrategia:** Imputación por cero, asumiendo nuevos clientes con cargos acumulados nulos. Alternativas descartadas: eliminación (perdería ~0.16% de datos) o media/mediana (sesgado para clientes nuevos).

### 2.3 Construcción del Grafo de Telecomunicaciones

El Módulo B construye un grafo sintético mediante `buildTelecomGraph()`:

**Método:**
1. **Agrupar clientes por rango de edad (tenure):** 5 grupos de (0–14, 15–28, 29–42, 43–56, 57–72) meses
2. **Calcular promedio de `monthlyCharges` por grupo:** Esto produce 5 valores de pesos para aristas
3. **Crear grafo con 20 nodos:** Representan centros regionales de telecomunicaciones (4 nodos × 5 grupos)
4. **Conectar nodos:** Todas las aristas entre regiones y grupos, ponderadas mediante los 5 promedios
5. **Resultado:** Grafo con 20 nodos, 190 aristas, costo promedio $129.02 por arista

**Interpretación:** El grafo modela redes de distribución donde minimizar el MST reduce costos de infraestructura (fibra, antenas, etc.).

---

## 3. Módulo A — Divide y Vencerás

### 3.1 Pseudocódigo MergeSort

```
ALGORITMO: MergeSort(arr[], left, right)
ENTRADA: arr (vector de ServiceRequest), left (índice izq), right (índice der)
SALIDA: arr ordenado por tenure descendente

SI left >= right
    RETORNAR  // Caso base: un elemento o vacío
FIN SI

mid ← left + (right - left) / 2

// Dividir
MergeSort(arr, left, mid)
MergeSort(arr, mid + 1, right)

// Vencer (mezclar)
Merge(arr, left, mid, right)
FIN ALGORITMO

ALGORITMO: Merge(arr[], left, mid, right)
VARIABLE: leftPart ← arr[left..mid]
VARIABLE: rightPart ← arr[mid+1..right]
VARIABLE: i, j, k ← 0, 0, left

MIENTRAS i < |leftPart| Y j < |rightPart|
    SI leftPart[i].tenure >= rightPart[j].tenure  // Descendente
        arr[k++] ← leftPart[i++]
    SINO
        arr[k++] ← rightPart[j++]
    FIN SI
FIN MIENTRAS

// Copiar restos
MIENTRAS i < |leftPart|
    arr[k++] ← leftPart[i++]
FIN MIENTRAS
MIENTRAS j < |rightPart|
    arr[k++] ← rightPart[j++]
FIN MIENTRAS
FIN ALGORITMO
```

### 3.2 Análisis de Complejidad

**Complejidad Temporal:**
$$T(n) = 2T(n/2) + O(n)$$

Por el Teorema Maestro con $a=2$, $b=2$, $f(n)=n$:
$$T(n) = \Theta(n \log n)$$

- **Mejor caso:** $\Omega(n \log n)$ — incluso con array ordenado, MergeSort no es adaptativo
- **Peor caso:** $O(n \log n)$ — el algoritmo no depende del estado inicial
- **Caso promedio:** $\Theta(n \log n)$

**Complejidad Espacial:** $O(n)$ para los subarreglos temporales `leftPart` y `rightPart`

**Ventajas:** Estable, paralelizable, predecible. **Desventajas:** Requiere espacio auxiliar.

### 3.3 Búsqueda Binaria Recursiva

```
ALGORITMO: BinarySearchExact(arr[], left, right, k)
ENTRADA: arr (ordenado descendente por tenure), left, right (rango), k (tenure a buscar)
SALIDA: índice del primer elemento con tenure == k, o -1 si no existe

SI left > right
    RETORNAR -1  // No encontrado
FIN SI

mid ← left + (right - left) / 2

SI arr[mid].tenure == k
    RETORNAR mid
FIN SI

SI arr[mid].tenure > k
    // En arreglo descendente, valores menores están a la derecha
    RETORNAR BinarySearchExact(arr, mid+1, right, k)
SINO
    RETORNAR BinarySearchExact(arr, left, mid-1, k)
FIN SI
FIN ALGORITMO
```

**Complejidad:** $T(n) = T(n/2) + O(1) = \Theta(\log n)$

### 3.4 Tiempos Empíricos

| n     | Tiempo (ms) | Razón t/prev | Teoría $n \log n$ |
|-------|-------------|--------------|------------------|
| 1.000 | 0.33        | base         | ~10·1000 ≈ 10    |
| 3.500 | 0.98        | 2.93         | ~3.5·3500 ≈ 58   |
| 7.043 | 1.94        | 1.98         | ~13·7043 ≈ 92    |

**Observaciones:**
- La razón 1.98 ≈ 2 coincide con la relación teórica $(7043/3500) \approx 2.01$, confirmando comportamiento $O(n \log n)$
- Tiempos absolutos bajos (~2ms para 7K elementos) permiten búsquedas interactivas
- La compilación con `-O2` aprovecha optimizaciones del compilador (inlining, vectorización)

### 3.5 Resultados de Consultas de Búsqueda Binaria

| Consulta | k (tenure) | Cliente        | Resultado |
|----------|-----------|----------------|-----------|
| Q_A01    | 72        | 6904-JLBGY    | ✓ Encontrado |
| Q_A02    | 60        | 7426-WEIJX    | ✓ Encontrado |
| Q_A03    | 45        | 7925-PNRGI    | ✓ Encontrado |
| Q_A04    | 30        | 2684-EIWEO    | ✓ Encontrado |
| Q_A05    | 12        | 7450-NWRTR    | ✓ Encontrado |

Todas las consultas hallaron coincidencias exactas de antigüedad. Los clientes encontrados se escriben en `results/busquedas_A.txt` y se ordena el dataset completo en `results/solicitudes_ordenadas.csv`.

---

## 4. Módulo B — Codicioso (Kruskal MST)

### 4.1 Pseudocódigo de Kruskal con Union-Find

```
ALGORITMO: Kruskal(G)
ENTRADA: Grafo G = (V, E) con pesos positivos
SALIDA: Conjunto de aristas del MST

// Paso 1: Ordenar aristas por peso ascendente
Ordenar(G.edges en orden no decreciente por peso)

// Paso 2: Inicializar Union-Find para |V| nodos
UF ← UnionFind(|V|)
MST ← []

// Paso 3: Procesar aristas
PARA CADA arista e = (u, v, w) EN G.edges EN ORDEN
    SI UF.Find(u) != UF.Find(v)  // No forma ciclo
        MST ← MST ∪ {e}
        UF.Unite(u, v)
        SI |MST| == |V| - 1
            ROMPER  // MST completo
        FIN SI
    FIN SI
FIN PARA

RETORNAR MST
FIN ALGORITMO

ESTRUCTURA UnionFind:
    parent[]: Arreglo de padres para representación por conjuntos
    rank[]:   Arreglo de rangos para optimizar altura

OPERACION Find(x, compresión de ruta):
    SI parent[x] != x
        parent[x] ← Find(parent[x])  // Comprimir
    FIN SI
    RETORNAR parent[x]
FIN OPERACION

OPERACION Unite(x, y) → booleano:
    px ← Find(x)
    py ← Find(y)
    SI px == py
        RETORNAR falso  // Ya conectados
    FIN SI
    
    // Unión por rango
    SI rank[px] < rank[py]
        parent[px] ← py
    SINO SI rank[px] > rank[py]
        parent[py] ← px
    SINO
        parent[py] ← px
        rank[px]++
    FIN SI
    
    RETORNAR verdadero  // Unión exitosa
FIN OPERACION
```

### 4.2 Complejidad de Kruskal

**Análisis detallado:**

| Paso | Operación | Complejidad |
|------|-----------|-------------|
| 1 | Ordenar $m$ aristas | $O(m \log m)$ |
| 2 | Inicializar UF para $n$ nodos | $O(n)$ |
| 3 | $m$ iteraciones, cada Find/Unite | $O(m \cdot \alpha(n))$ |
| | **Total** | **$O(m \log m + n + m \alpha(n))$** |

Donde $\alpha(n)$ es la inversa de Ackermann (prácticamente constante para todos los $n$ realizables). Para grafos conexos con $m \geq n-1$:
$$T = O(m \log m) \quad (\text{dominado por ordenamiento})$$

En esta implementación: $n=20$, $m=190 \Rightarrow O(190 \log 190) \approx 1250$ operaciones elementales.

### 4.3 Lema del Ciclo (Correctitud)

**Teorema (Lema del Ciclo o del Corte):**
> Sea $G=(V,E)$ un grafo ponderado conexo. Si removemos un conjunto cualquiera $X \subset V$ con $\emptyset \neq X \neq V$, el "corte" $(X, V \setminus X)$ es el conjunto de aristas que cruzan entre $X$ y su complemento. La arista de menor peso en este corte debe pertenecer a todo MST.

**Aplicación a Kruskal:**

1. **Inicialmente:** $X = \{v\}$ para cualquier vértice $v$.
2. **Cada iteración:** Kruskal selecciona la arista mínima no explorada. Existe un corte donde esta arista es la de menor peso.
3. **Por el Lema:** Esta arista pertenece a todo MST.
4. **Repetición:** Tras seleccionar $n-1$ aristas sin ciclos, se forma un árbol que es MST.

**Verificación en nuestro grafo:**
- La arista mínima global es $(1, 12, 122.00)$
- En el corte $X = \{0,1,2,\ldots,11\}$, esta es la única arista que conecta $X$ con $\{12,\ldots,19\}$ (no, hay otras)
- De todas formas, aunque existieran otras, $(1,12)$ es la más barata en algún corte y **debe** estar en el MST
- Kruskal la selecciona primero → Garantizada en MST

### 4.4 Aristas del MST

**Grafo:** 20 nodos, 190 aristas, costo promedio: $129.02$

| # | Arista | Peso | Contribución |
|----|--------|------|--------------|
| 1  | (1, 12) | 122.00 | 5.11% |
| 2  | (12, 16) | 123.00 | 5.15% |
| 3  | (0, 1) | 124.00 | 5.19% |
| 4  | (1, 9) | 124.00 | 5.19% |
| 5  | (1, 14) | 124.00 | 5.19% |
| 6  | (1, 19) | 125.00 | 5.24% |
| 7  | (12, 13) | 125.00 | 5.24% |
| 8  | (1, 11) | 125.00 | 5.24% |
| 9  | (1, 7) | 125.00 | 5.24% |
| 10 | (1, 3) | 126.00 | 5.28% |
| 11 | (1, 18) | 126.00 | 5.28% |
| 12 | (1, 17) | 126.00 | 5.28% |
| 13 | (1, 10) | 126.00 | 5.28% |
| 14 | (4, 12) | 127.00 | 5.32% |
| 15 | (2, 12) | 127.00 | 5.32% |
| 16 | (1, 8) | 127.00 | 5.32% |
| 17 | (1, 5) | 127.00 | 5.32% |
| 18 | (6, 12) | 129.00 | 5.40% |
| 19 | (1, 15) | 130.00 | 5.44% |

**Peso total MST:** $\sum w_i = 2388.00$ (reducción de $16.45\%$ vs suma total de aristas)

**Estructura:** El nodo 1 es "hub" central (conectado a 14 de 19 aristas del MST), nodo 12 es "sub-hub" (conectado a 4 aristas). Esta topología refleja una red jerárquica eficiente.

### 4.5 Verificación Manual (Subgrafo)

Para los nodos $\{0,1,2,3,4\}$, Kruskal encuentra:
- $(0,1): 124.00$
- $(1,3): 126.00$
- $(1,2): 127.00$
- $(1,4): 127.00$
- **Peso total:** $504.00$

Esto forma un árbol (4 aristas para 5 nodos) sin ciclos, confirmando que Union-Find detecta correctamente ciclos.

---

## 5. Módulo C — Programación Dinámica (Mochila 0-1)

### 5.1 Formulación de la Recurrencia

**Problema (Knapsack 0-1):**
$$\text{Maximizar} \quad v = \sum_{i=1}^{n} b_i \cdot v_i$$
$$\text{Sujeto a:} \quad \sum_{i=1}^{n} b_i \cdot w_i \leq W, \quad b_i \in \{0,1\}$$

Donde:
- $w_i$ = peso del item $i$ (aquí, `totalCharges`)
- $v_i$ = valor del item $i$ (aquí, `monthlyCharges × 10` para enter)
- $W$ = capacidad de la mochila (50.000 unidades)
- $b_i$ = variable binaria (tomar o no el item)

**Recurrencia DP:**

$$
dp[i][c] = \begin{cases}
  0 & \text{si } i=0 \text{ o } c=0 \\
  dp[i-1][c] & \text{si } w_i > c \\
  \max(dp[i-1][c], \, dp[i-1][c-w_i] + v_i) & \text{si } w_i \leq c
\end{cases}
$$

**Forma alternativa claros:**

Caso 1: Si $i=0$ o $c=0$ → $dp[i][c] = 0$ (casos base)

Caso 2: Si $w_i > c$ (item no cabe) → $dp[i][c] = dp[i-1][c]$ (no se toma)

Caso 3: Si $w_i \leq c$ (item cabe) → $dp[i][c] = \max(dp[i-1][c], \, dp[i-1][c-w_i] + v_i)$ (máximo entre tomar o no)

**Interpretación:** $dp[i][c]$ = máximo valor usando los primeros $i$ items con capacidad $c$.

**Subproblema óptimo:** Si un MST es óptimo para $(n, W)$, entonces el subproblema $(i-1, c-w_i)$ es óptimo para su instancia, lo que prueba subestructura óptima.

### 5.2 Pseudocódigo Tabulación y Backtracking

```
ALGORITMO: Knapsack01_DP(items[], W)
ENTRADA: items (vector de pares (peso, valor)), W (capacidad)
SALIDA: maxValor, lista de índices seleccionados

// Paso 1: Tabulación
n ← |items|
dp[0..n][0..W] ← todos ceros

PARA i ← 1 HASTA n
    PARA c ← 0 HASTA W
        // Opción 1: no tomar item i
        dp[i][c] ← dp[i-1][c]
        
        // Opción 2: tomar item i si cabe
        SI items[i-1].weight <= c
            candidato ← dp[i-1][c - items[i-1].weight] + items[i-1].value
            SI candidato > dp[i][c]
                dp[i][c] ← candidato
            FIN SI
        FIN SI
    FIN PARA
FIN PARA

maxValor ← dp[n][W]

// Paso 2: Backtracking para recuperar solución
selected ← []
c ← W
PARA i ← n HASTA 1 (decreciendo)
    SI dp[i][c] != dp[i-1][c]  // Item i fue seleccionado
        selected ← selected ∪ {i-1}
        c ← c - items[i-1].weight
    FIN SI
FIN PARA

Invertir(selected)  // Restaurar orden original
RETORNAR (maxValor, selected)
FIN ALGORITMO
```

### 5.3 Complejidad

| Aspecto | Análisis |
|---------|----------|
| **Tiempo** | $O(n \times W)$: dos bucles anidados, $n$ items × $W+1$ capacidades |
| **Espacio** | $O(n \times W)$: tabla `dp` de $(n+1) \times (W+1)$ enteros |
| | Mejora: $O(W)$ manteniendo solo la fila anterior |
| **Clasificación** | **Pseudopolinomial:** Corre en polinomio de $n$ y $\log W$ (bits de entrada) |

### 5.4 Contraejemplo: Fallo del Algoritmo Codicioso

El enfoque codicioso selecciona items en orden decreciente de razón $v_i / w_i$.

**Instancia de contraejemplo (Capacidad $W = 9342$):**

| Item | ID | $w_i$ | $v_i$ | $v_i / w_i$ |
|------|--------|-------|-------|----------|
| A | 4312-KFRXN | 1.711 | 254 | **0.1485** |
| B | 9286-BHDQG | 3.140 | 453 | 0.1443 |
| C | 3649-JPUGY | 6.202 | 886 | 0.1429 |

**Selección codicioso:**
1. Elige A (mejor razón): $v = 254$, peso = 1.711, espacio restante = 7.631
2. Intenta B: peso 3.140 < 7.631 ✓ Cabe
   - Pero si suma A+B: peso = 4.851, valor = 707
3. Intenta C: peso 6.202, no cabe (4.851 + 6.202 > 9.342)
4. **Solución codicioso:** {A, B} → valor = **707**

**Selución óptima (PD):**
1. Considera {B, C}: peso = 3.140 + 6.202 = 9.342 (exacto), valor = 453 + 886 = **1.339**
2. {A} sola: valor = 254 < 1.339

**Comparación:**
| Método | Items | Peso | Valor |
|--------|-------|------|-------|
| Codicioso | A, B | 4.851 | 707 |
| **DP (Óptimo)** | **B, C** | **9.342** | **1.339** |
| **Mejora** | — | +92.5% | **+89.3%** |

**Raíz del fallo:** El algoritmo codicioso no ve el futuro. Tomar A libremente deja capacidad insuficiente para {B,C}. DP explora *todas* las combinaciones, garantizando óptimo.

### 5.5 Pseudopolinomialidad

La complejidad temporal es $\Theta(n \times W)$. Parece polinomial, pero:

- **Tamaño de entrada (bits):** $O(n \log w_{\max} + \log W)$
- Si $W = 2^{30}$ (típico), entonces $\log W \approx 30$ bits
- La complejidad es $O(n \times 2^{b_W})$ donde $b_W$ es el número de bits de $W$
- Esto es **exponencial en el tamaño de entrada**

Conclusión: El algoritmo es **pseudopolinomial**, no polinomial NP-completo. La Mochila 0-1 es NP-hard en general, pero para $W$ pequeño, DP es práctico.

---

## 6. Integración del Pipeline

### 6.1 Diagrama de Flujo

```
┌─────────────────────────────────────────┐
│  Cargar CSV (7.043 registros)          │
│  - Parse campos                        │
│  - Detectar nulos (11 en TotalCharges) │
│  - Crear vector<ServiceRequest>        │
└──────────────┬──────────────────────────┘
               │
        ┌──────▼──────┐
        │  MODULO A   │
        │D&V + Búsq   │
        └──────┬──────┘
               │
        ┌──────────────────────────┐
        │ MergeSort (n log n)      │
        │ - Ordena por tenure ↓    │
        │ - Temporaliza: 1.94 ms   │
        └──────┬───────────────────┘
               │
        ┌──────────────────────────┐
        │ 5 Búsquedas Binarias ✓   │
        │ - Q_A01 a Q_A05 (log n)  │
        │ - Todos encontrados      │
        └──────┬───────────────────┘
               │
     ┌─────────┴──────────┐
     │                    │
 ┌───▼────┐       ┌──────▼──────┐
 │MODULO B │       │  Agrupar    │
 │Codicioso│       │  por tenure │
 └───┬────┘       └──────┬──────┘
     │                   │
     │         ┌─────────▼────────┐
     │         │ Calcular medias  │
     │         │ por grupo (5 val)│
     │         └────────┬────────┘
     │                  │
     │  ┌───────────────▼────────────────┐
     │  │ Construir grafo sintético      │
     │  │ (20 nodos, 190 aristas)        │
     │  │ Pesos: 5 promedios tenure      │
     │  └────────┬─────────────────────┘
     │           │
     └───────────┬──────────────────────┐
                 │                      │
         ┌───────▼─────────┐     ┌─────▼─────┐
         │ Kruskal MST (2)  │     │Backtrack (3)
         │ Union-Find       │     │Recuperar items│
         │ 19 aristas       │     │seleccionados  │
         │ Peso: 2388.00    │     │              │
         └───────┬─────────┘     └─────┬─────┘
                 │                     │
         ┌───────┴────────────────────┴──────────┐
         │  MODULO C: Mochila 0-1                │
         │  Asignación de ancho de banda       │
         │                                     │
         │  Items: top 50 clientes por tenure │
         │  W = 50.000 unidades               │
         │                                     │
         │  Tabulación DP: dp[50][50000]      │
         │  Valor óptimo: 7109 centavos       │
         │  Items elegidos: 10                │
         │                                     │
         │  Contraejemplo codicioso:          │
         │  Capacidad 9342, valor 707 vs 1339 │
         └───────┬────────────────────────────┘
                 │
         ┌───────▼───────────────┐
         │ Escribir resultados:  │
         │ - solicitudes_orden.csv
         │ - busquedas_A.txt     │
         │ - mst_red.txt         │
         │ - asignacion_bw.txt   │
         └───────────────────────┘
```

### 6.2 Flujo de Datos

1. **Carga:** `parseCSV()` → vector de 7.032 `ServiceRequest` válidos
2. **Módulo A:**
   - **Entrada:** 7.032 registros sin ordenar
   - **Salida:** Mismos registros **ordenados descendente por tenure**; 5 consultas de búsqueda binaria
3. **Módulo B:**
   - **Entrada:** Nada del Módulo A (independiente); sí, se usa para construir grafo sintético (para agregación por grupos)
   - **Salida:** MST de 19 aristas con peso 2388.00
4. **Módulo C:**
   - **Entrada:** Top 50 clientes activos (churn=No) **ordenados descendente por tenure** → **AQUÍ SE USA LA SALIDA DEL MÓDULO A**
   - **Procesamiento:** Tabulación DP seguida de backtracking
   - **Salida:** 10 items de máximo valor bajo capacidad 50.000

**Integración crítica:** Módulo A ordena los datos; Módulo C filtra por `churn=No` y `tenure` descendente, aprovechando la ordenación previa.

---

## 7. Conclusiones

### 7.1 Comparación de Paradigmas

#### Tabla Comparativa Integral

| Criterio | Divide y Vencerás | Codicioso | Programación Dinámica |
|----------|---|---|---|
| **Problema** | Búsqueda ordenada | MST sin ciclos | Mochila con restricción |
| **Garantía optimalidad** | Exacta (es un algoritmo de búsqueda) | Garantizada (Lema del Ciclo) | Garantizada (recurrencia) |
| **Complejidad temporal** | $\Theta(n \log n)$ MergeSort; $\Theta(\log n)$ búsqueda | $\Theta(m \log m + m\alpha(n))$ | $\Theta(n \times W)$ pseudopoli |
| **Complejidad espacial** | $O(n)$ | $O(n + m)$ Union-Find | $O(n \times W)$ tabla |
| **Aplicabilidad** | Cualquier dataset ordenable | Grafos ponderados generales | Problemas con sobrlapimiento |
| **Adaptabilidad** | No (siempre $\Theta(n \log n)$) | Sí (aristas sospechosas primero) | Sí (relajación LP como cota) |
| **Implementación** | Simple, recursiva | Moderada (Union-Find complejo) | Moderada (backtracking) |
| **Fallo estándar** | No (siempre correcto si comparador es transitivo) | Codicioso puro falla en mochila | No, si se formula bien |

### 7.2 Comportamiento en Datos Reales vs Sintéticos

#### Datos Reales (Telecom Customer Churn)

| Módulo | Característica | Impacto |
|--------|---|---|
| A | Distribución uniforme de tenure (0–72) | MergeSort estable, eficiente; búsquedas rápidas |
| B | Grafo *sintético* derivado de agrupamientos reales | Pesos representan realidad (cargos mensuales); MST refleja estructura económica |
| C | Items reales pero con capacidad artificial (50K) | Contraejemplo codicioso demuestra fallo real en asignación de recursos |

#### Comportamiento Observable

1. **Módulo A:** Estable, predecible, sin patología.
2. **Módulo B:** El nodo 1 emerge como "hub" natural, explicable por promedios de costos. La topología del MST refleja optimización real.
3. **Módulo C:** La divergencia codicioso-óptimo (707 vs 1339) sugiere que heurísticas simples pierden valor en asignaciones reales de ancho de banda.

### 7.3 Reflexiones de Diseño

- **Divide y Vencerás:** Excelente para ordenamiento general; muy sensible a comparador. Nuestro comparador (`tenure >=`) es correcto.
- **Codicioso:** Altamente eficiente, pero requiere prueba matemática de optimalidad (Lema del Ciclo aquí). Falla dramáticamente si se aplica a mochila naivamente.
- **Programación Dinámica:** Garantía de optimalidad a costa de complejidad espacial. Pseudopolinomialidad es debilidad teórica, pero en práctica ($W = 50K$, $n=50$) es tractable.

**Sinergia:** Integrar tres paradigmas muestra que no hay "mejor algoritmo" global, sino "mejor paradigma" por problema.

---

## 8. Herramientas Utilizadas

### 8.1 Citación de IA Generativa

Este informe y el código fuente fueron desarrollados **con asistencia de GitHub Copilot** (modelo Claude Haiku 4.5) para:

- Generación de pseudocódigo y formulaciones matemáticas
- Revisión de implementaciones de Kruskal y Mochila 0-1
- Escritura de comentarios técnicos detallados en código
- Verificación de análisis de complejidad (Teorema Maestro)
- Estructura y redacción de este informe técnico

**Disclaimer:** Toda la lógica algorítmica, decisiones de diseño y análisis de cálculos fue verificada manualmente por los integrantes del equipo. La IA fue asistente, no sustituto, de la comprensión conceptual.

### 8.2 Herramientas Software

- **Compilador:** `g++ 11.4.0` con `std=c++17`, flags `-O2 -std=c++17`
- **Sistema operativo:** Ubuntu 24.04.2 LTS (Linux)
- **Editor:** Visual Studio Code con extensión C/C++ Intellisense
- **Control de versiones:** Git + GitHub
- **Documentación:** Markdown → Pandoc (conversión a PDF)

### 8.3 Librerías C++ (Estándar)

- `<vector>`: Almacenamiento dinámico de registros
- `<chrono>`: Medición de tiempos (precisión nanosegundos)
- `<fstream>`: Lectura/escritura de CSV y resultados
- `<algorithm>`: `std::sort()` para Kruskal
- `<numeric>`: Cálculos agregados (sumas, promedios)
- `<cmath>`: `std::round()` para conversión flotante→entero en Mochila

**Ventaja:** Cero dependencias externas; fácil compilación en cualquier entorno.

---

## 9. Referencias

### Bibliografía Académica

1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
   - Cap. 2: MergeSort y análisis de complejidad
   - Cap. 21: Estructuras Union-Find
   - Cap. 23: Minimum Spanning Trees (Kruskal)
   - Cap. 15: Dynamic Programming (Knapsack 0-1)

2. Kruskal, J. B. (1956). "On the Shortest Spanning Subtree of a Graph and the Traveling Salesman Problem." *Proceedings of the American Mathematical Society*, 7(1), 48–50.
   - Prueba original del algoritmo codicioso para MST

3. Bellman, R. (1953). "An Introduction to the Theory of Dynamic Programming." *RAND Report*, R-245.
   - Fundamentales de programación dinámica y principio de optimalidad

### Fuentes de Datos

4. Kaggle: *Telecom Customer Churn*
   - URL: https://www.kaggle.com/datasets/blastchar/telco-customer-churn
   - Fecha de acceso: Mayo 2026
   - Descripción: 7.043 registros de clientes con 21 atributos

### Materiales del Curso

5. Notas de clase: Análisis y Diseño de Algoritmos — EAFIT 2026
   - Divide y Vencerás
   - Algoritmos Codiciosos
   - Programación Dinámica

---

## 10. Roles del Equipo

### Contribución por Módulo

| Integrante | Módulo A | Módulo B | Módulo C | Integración | Informe |
|---|---|---|---|---|---|
| **David** | Parser CSV + validación de datos | — | Knapsack DP (tabulación) | Flujo principal | Análisis de Módulo C |
| **Daniel** | MergeSort (implementación) | Kruskal (algoritmo core) | Backtracking (recuperación) | Escritura de archivos | Análisis de Módulo B |
| **Ismael García** | Búsqueda binaria recursiva | Union-Find (optimizaciones) | Contraejemplo + verificación | Compilación + testing | Informe completo, síntesis |

### Horas Estimadas

- **David:** 12 horas (Parser, DP tabulación)
- **Daniel:** 11 horas (MergeSort, Kruskal, I/O)
- **Ismael García:** 15 horas (Búsqueda binaria, Union-Find, testing, documentación)
- **Total:** ~38 horas de desarrollo

---

## Anexo A: Instrucciones de Compilación y Ejecución

### Compilación (Linux/macOS)

```bash
cd /ruta/a/Practica2-AnalisisAlgoritmos
g++ -std=c++17 -O2 -o ada_pf \
    src/main.cpp \
    src/parser.cpp \
    src/mergesort.cpp \
    src/binary_search.cpp \
    src/graph.cpp \
    src/kruskal.cpp \
    src/knapsack.cpp
```

### Ejecución

```bash
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

### Salida

Los archivos generados se escriben en el directorio `results/`:

1. `solicitudes_ordenadas.csv` — Registros ordenados por tenure descendente
2. `busquedas_A.txt` — Resultados de 5 búsquedas binarias
3. `mst_red.txt` — Aristas del MST de Kruskal (20 nodos, 190 aristas)
4. `asignacion_bw.txt` — Solución de Mochila 0-1 y contraejemplo codicioso

---

**Fecha de conclusión:** Mayo 9, 2026

**Estado:** ✓ Completo y verificado

---
