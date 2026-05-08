#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Split a CSV line respecting the fact that no field is quoted here
static std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

std::vector<ServiceRequest> parseCSV(const std::string& filepath, int& nullCount) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    std::vector<ServiceRequest> records;
    nullCount = 0;
    std::string line;

    // Skip header
    std::getline(file, line);

    // Column indices (0-based) in the dataset:
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
        if (line.empty()) continue;
        auto fields = splitCSV(line);
        if (fields.size() < 21) continue;

        ServiceRequest req;
        req.customerID = fields[0];

        try { req.tenure = std::stoi(fields[5]); }
        catch (...) { req.tenure = 0; }

        try { req.monthlyCharges = std::stod(fields[18]); }
        catch (...) { req.monthlyCharges = 0.0; }

        // TotalCharges can be blank for tenure=0 customers
        std::string tc = fields[19];
        // trim whitespace
        size_t start = tc.find_first_not_of(" \t\r\n");
        if (start == std::string::npos || tc.empty()) {
            req.totalCharges = 0.0;
            nullCount++;
        } else {
            try { req.totalCharges = std::stod(tc); }
            catch (...) { req.totalCharges = 0.0; nullCount++; }
        }

        req.churn = (fields[20] == "Yes");
        records.push_back(req);
    }

    return records;
}
