#include "Mobility.h"
#include <iomanip>
#include <algorithm>

// Constructor with parameters (calls base constructor to initialize members)
Mobility::Mobility(int id, const std::string& name, const Date& date,
                   double cost, int life, double salvage, const std::string& mode)
    : Equipment(id, name, date, cost, life, salvage), operationMode(mode) {}

Mobility::~Mobility() {}

// Accessors
std::string Mobility::getOperationMode() const { return operationMode; }
void Mobility::setOperationMode(const std::string& mode) { operationMode = mode; }


void Mobility::printDetails(std::ostream& out) const {
    Equipment::printDetails(out);
    out << "  " << operationMode << "\n";
}

// Double-declining balance depreciation schedule
void Mobility::depreciate(std::ostream& out) const {
    out << "---------------------------------------------------------------------\n";
    out << "                            Depreciation Schedule\n";
    out << "---------------------------------------------------------------------\n";
    out << "Double-declining Balance Depreciation\n";
    out << "Equipment ID: " << equipmentID << "\n";
    out << "Equipment name: " << equipmentName << "\n";
    out << "Original cost: $" << std::fixed << std::setprecision(2) << originalCost << "\n";
    out << "Salvage value: $" << std::fixed << std::setprecision(2) << salvageValue << "\n";
    out << "Useful life (in years): " << usefulLife << "\n\n";

    out << std::left << std::setw(6) << "Period" << std::setw(22) << "Depreciation Expense ($)" << "Book Value ($)\n";
    out << std::fixed << std::setprecision(2);

    produceSchedule(out, equipmentName, 1, originalCost, salvageValue, usefulLife, 0.0);
    out << "\n---------------------------------------------------------------------\n";
}

// Recursive helper for double-declining balance depreciation schedule
void Mobility::produceSchedule(std::ostream& out, const std::string& assetName,
    int period, double bookValue, double salvage, int usefulLife,
    double totalDepreciation) const {
    // Convert to single-return style: no early return; use if/else and one return at end
    if (period > usefulLife) {
        out << "\nTotal Depreciation: " << std::fixed << std::setprecision(2) << totalDepreciation << "\n";
    } else {
        // double-declining rate
        double rate = 2.0 / static_cast<double>(std::max(1, usefulLife));
        double expense = bookValue * rate;

        // ensure we do not go below salvage
        if ((bookValue - expense) < salvage) {
            expense = bookValue - salvage;
            if (expense < 0.0) expense = 0.0;
        }

        out << std::right << std::setw(4) << period << "   "
            << std::setw(20) << expense << "   "
            << std::setw(10) << bookValue << "\n";

        totalDepreciation += expense;
        double nextBook = bookValue - expense;
        produceSchedule(out, assetName, period + 1, nextBook, salvage, usefulLife, totalDepreciation);
    }
}