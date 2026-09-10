#include "Nurse.h"
#include <iomanip>
// Constructor with parameters (calls base constructor to initialize Employee members)
Nurse::Nurse(int id, const std::string& first, const std::string& last, float wage, const std::string& desig)
    : Employee(id, first, last), hourlyWage(wage), designation(desig) {}
// Destructor
Nurse::~Nurse() {}
// Accessors
float Nurse::getHourlyWage() const { return hourlyWage; }
std::string Nurse::getDesignation() const { return designation; }
// Mutators
void Nurse::setHourlyWage(float wage) { hourlyWage = wage; }
void Nurse::setDesignation(const std::string& desig) { designation = desig; }
// Print details in formatted manner (override base class method)
void Nurse::printDetails(std::ostream& os) const {
    os << std::left << std::setw(5) << getEmployeeID() << " "
       << std::setw(20) << (getFirstName() + " " + getLastName())
       << std::fixed << std::setprecision(2) << std::setw(10) << hourlyWage
       << " " << designation;
}