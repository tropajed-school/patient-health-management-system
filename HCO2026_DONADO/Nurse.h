#pragma once
#include "Employee.h"
#include <string>

// Nurse class derived from Employee
class Nurse : public Employee {
private:
    float hourlyWage;
    std::string designation;
public:
    // five-argument constructor with default parameters
    Nurse(int id = 0, const std::string& first = "", const std::string& last = "",
          float wage = 0.0f, const std::string& desig = "");
    ~Nurse();

    // accessors
    float getHourlyWage() const;
    std::string getDesignation() const;

    // mutators
    void setHourlyWage(float wage);
    void setDesignation(const std::string& desig);

    // printable details 
    void printDetails(std::ostream& os) const;
};