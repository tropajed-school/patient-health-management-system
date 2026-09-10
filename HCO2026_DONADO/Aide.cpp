#include "Aide.h"
#include <iomanip>
// Constructor with parameters (calls base constructor to initialize Employee members)
Aide::Aide(int id, const std::string& first, const std::string& last, float wage, const std::string& cert)
    : Employee(id, first, last), hourlyWage(wage), certification(cert) {}
// Destructor
Aide::~Aide() {}
// Accessors
float Aide::getHourlyWage() const { return hourlyWage; }
std::string Aide::getCertification() const { return certification; }
// Mutators
void Aide::setHourlyWage(float wage) { hourlyWage = wage; }
void Aide::setCertification(const std::string& cert) { certification = cert; }
// Print details in formatted manner (override base class method)
void Aide::printDetails(std::ostream& os) const {
    os << std::left << std::setw(5) << getEmployeeID() << " "
       << std::setw(20) << (getFirstName() + " " + getLastName())
       << std::fixed << std::setprecision(2) << std::setw(10) << hourlyWage
       << " " << certification;
}