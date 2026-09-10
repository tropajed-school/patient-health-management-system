#include "CaseManager.h"
#include <iomanip>

// Constructor with parameters (calls base constructor to initialize Employee members)
CaseManager::CaseManager(int id, const std::string& first, const std::string& last, float sal, const std::string& deg)
    : Employee(id, first, last), salary(sal), degree(deg) {}
// Destructor
CaseManager::~CaseManager() {}
// Accessors
float CaseManager::getSalary() const { return salary; }
std::string CaseManager::getDegree() const { return degree; }
// Mutators
void CaseManager::setSalary(float sal) { salary = sal; }
void CaseManager::setDegree(const std::string& deg) { degree = deg; }
// Print details in formatted manner (override base class method)
void CaseManager::printDetails(std::ostream& os) const {
    os << std::left << std::setw(5) << getEmployeeID() << " "
       << std::setw(20) << (getFirstName() + " " + getLastName())
       << std::fixed << std::setprecision(2) << std::setw(12) << salary
       << " " << degree;
}