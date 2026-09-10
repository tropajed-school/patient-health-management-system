#include "Monitor.h"
#include <iomanip>

// Constructor with parameters (calls base constructor to initialize members)
Monitor::Monitor(int id, const std::string& name, const Date& date,
                 double cost, int life, double salvage, const std::string& display)
    : Equipment(id, name, date, cost, life, salvage), displayType(display) {}

Monitor::~Monitor() {}

// Accessors
std::string Monitor::getDisplayType() const { return displayType; }
void Monitor::setDisplayType(const std::string& dt) { displayType = dt; }

// Print details in formatted manner 
void Monitor::printDetails(std::ostream& out) const {
    Equipment::printDetails(out);
    out << "  " << displayType << "\n";
}
// Straight-line depreciation schedule
void Monitor::depreciate(std::ostream& out) const {
    out << "---------------------------------------------------------------------\n";
    out << "                            Depreciation Schedule\n";
    out << "---------------------------------------------------------------------\n";
    out << "Straight-line Depreciation\n";
    out << "Equipment ID: " << equipmentID << "\n";
    out << "Equipment name: " << equipmentName << "\n";
    out << "Original cost: $" << std::fixed << std::setprecision(2) << originalCost << "\n";
    out << "Salvage value: $" << std::fixed << std::setprecision(2) << salvageValue << "\n";
    out << "Useful life (in years): " << usefulLife << "\n\n";

    double annualExpense = 0.0;
    if (usefulLife > 0) {
        annualExpense = (originalCost - salvageValue) / static_cast<double>(usefulLife);
    }

    out << std::left << std::setw(6) << "Period" << std::setw(22) << "Depreciation Expense ($)" << "Book Value ($)\n";
    out << std::fixed << std::setprecision(2);

    // Start recursion with period 1 and bookValue = originalCost, totalDepreciation 0.0
    produceSchedule(out, equipmentName, 1, originalCost, salvageValue, usefulLife, annualExpense, 0.0);

    out << "\n---------------------------------------------------------------------\n";
}

void Monitor::produceSchedule(std::ostream& out, const std::string& assetName,
                              int period, double bookValue, double salvage, int usefulLife,
                              double annualExpense, double totalDepreciation) const {
    // Single-return style: no early return; use if/else and one return at end
    if (period > usefulLife || usefulLife <= 0) {
        out << "\nTotal Depreciation: " << std::fixed << std::setprecision(2) << totalDepreciation << "\n";
    }
    else {
        double expense = annualExpense;
        // ensure we do not depreciate below salvage in last period
        if ((bookValue - expense) < salvage) {
            expense = bookValue - salvage;
            if (expense < 0.0) expense = 0.0;
        }

        out << std::right << std::setw(4) << period << "   "
            << std::setw(20) << expense << "   "
            << std::setw(10) << bookValue << "\n";

        totalDepreciation += expense;
        double nextBook = bookValue - expense;
        produceSchedule(out, assetName, period + 1, nextBook, salvage, usefulLife, annualExpense, totalDepreciation);
    }
}