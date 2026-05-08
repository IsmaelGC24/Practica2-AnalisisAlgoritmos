#pragma once
#include <string>
#include <vector>

struct ServiceRequest {
    std::string customerID;
    int         tenure;
    double      monthlyCharges;
    double      totalCharges;
    bool        churn; // true = Yes (at risk), false = No (active)
};

// Loads records from CSV. Fills nullCount with number of blank TotalCharges.
std::vector<ServiceRequest> parseCSV(const std::string& filepath, int& nullCount);
