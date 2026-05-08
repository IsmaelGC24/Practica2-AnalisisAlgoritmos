#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Dividir una linea CSV asumiendo que ningun campo esta entre comillas aqui
static std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

// Parametros: filepath (ruta del archivo CSV), nullCount (referencia a contador de nulos)
std::vector<ServiceRequest> parseCSV(const std::string& filepath, int& nullCount) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("No se puede abrir el archivo: " + filepath);
    }

    std::vector<ServiceRequest> records;
    nullCount = 0;
    std::string line;

    // Saltar la cabecera (primera línea)
    std::getline(file, line);

    // Índices de columnas (base 0) en el conjunto de datos:
    // 0  customerID
    // 1  gender
    // 2  SeniorCitizen
    // 3  Partner
    // 4  Dependents
    // 5  tenure
    // 6  PhoneService
    // 7  MultipleLines
    // 8  InternetService
    // 9  OnlineSecurity
    // 10 OnlineBackup
    // 11 DeviceProtection
    // 12 TechSupport
    // 13 StreamingTV
    // 14 StreamingMovies
    // 15 Contract
    // 16 PaperlessBilling
    // 17 PaymentMethod
    // 18 MonthlyCharges
    // 19 TotalCharges
    // 20 Churn

    while (std::getline(file, line)) {
        if (line.empty()) continue;           // Saltar líneas vacías
        auto fields = splitCSV(line);
        if (fields.size() < 21) continue;     // Saltar líneas incompletas

        ServiceRequest req;
        req.customerID = fields[0];            // Columna 0: ID del cliente

        // Columna 5: tenure (antigüedad en meses)
        try { req.tenure = std::stoi(fields[5]); }
        catch (...) { req.tenure = 0; }

        // Columna 18: monthlyCharges (cargo mensual)
        try { req.monthlyCharges = std::stod(fields[18]); }
        catch (...) { req.monthlyCharges = 0.0; }

        // TotalCharges puede estar vacío para clientes con tenure=0
        std::string tc = fields[19];
        // Eliminar espacios en blanco al inicio y final
        size_t start = tc.find_first_not_of(" \t\r\n");
        if (start == std::string::npos || tc.empty()) {
            req.totalCharges = 0.0;            // Campo vacío o solo espacios
            nullCount++;                       // Contar como nulo
        } else {
            try { req.totalCharges = std::stod(tc); }
            catch (...) { req.totalCharges = 0.0; nullCount++; }
        }

        req.churn = (fields[20] == "Yes");     // Columna 20: Churn (abandono)
        records.push_back(req);
    }

    return records;
}