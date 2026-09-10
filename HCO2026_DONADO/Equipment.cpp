#include "Equipment.h"
#include <iomanip>

// Constructor with parameters (calls base constructor to initialize members)
Equipment::Equipment(int id, const std::string& name, const Date& date,
                     double cost, int life, double salvage)
    : equipmentID(id), equipmentName(name), purchaseDate(date),
      originalCost(cost), usefulLife(life), salvageValue(salvage) {}

Equipment::~Equipment() {}

// Accessors
int Equipment::getEquipmentID() const { return equipmentID; }
std::string Equipment::getEquipmentName() const { return equipmentName; }
Date Equipment::getPurchaseDate() const { return purchaseDate; }
double Equipment::getOriginalCost() const { return originalCost; }
int Equipment::getUsefulLife() const { return usefulLife; }
double Equipment::getSalvageValue() const { return salvageValue; }

// Mutators
void Equipment::setEquipmentID(int id) { equipmentID = id; }
void Equipment::setEquipmentName(const std::string& name) { equipmentName = name; }
void Equipment::setPurchaseDate(const Date& date) { purchaseDate = date; }
void Equipment::setOriginalCost(double cost) { originalCost = cost; }
void Equipment::setUsefulLife(int life) { usefulLife = life; }
void Equipment::setSalvageValue(double salvage) { salvageValue = salvage; }

// Print details in formatted manner 
void Equipment::printDetails(std::ostream& out) const {
    out << std::left << std::setw(5) << equipmentID << " "
        << std::setw(20) << equipmentName << " "
        << std::setw(10) << purchaseDate.toString() << " "
        << std::right << std::setw(10) << std::fixed << std::setprecision(2) << originalCost << " "
        << std::setw(4) << usefulLife << " "
        << std::setw(10) << std::fixed << std::setprecision(2) << salvageValue;
}
// Compare by equipmentID for sorting
bool Equipment::operator<(const Equipment& other) const {
    return equipmentID < other.equipmentID;
}