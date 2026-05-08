#pragma once
#include <string>
#include <vector>

struct ServiceRequest {
    std::string customerID;    // ID del cliente
    int         tenure;        // Antiguedad en meses
    double      monthlyCharges; // Cargo mensual
    double      totalCharges;   // Cargo total
    bool        churn;          // true = Si (en riesgo de abandono), false = No (activo)
};

// Carga los registros desde un archivo CSV. Rellena nullCount con la cantidad de TotalCharges vacios.
// Parametros: filepath (ruta del archivo CSV), nullCount (referencia a contador de nulos)
std::vector<ServiceRequest> parseCSV(const std::string& filepath, int& nullCount);
