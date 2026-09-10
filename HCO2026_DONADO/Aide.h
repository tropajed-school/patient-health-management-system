#pragma once
#include "Employee.h"
#include <string>

class Aide : public Employee {
private:
    float hourlyWage;
    std::string certification;
public:
    // five-argument constructor with default parameters
    Aide(int id = 0, const std::string& first = "", const std::string& last = "",
         float wage = 0.0f, const std::string& cert = "");
    ~Aide();

    // accessors
    float getHourlyWage() const;
    std::string getCertification() const;

    // mutators
    void setHourlyWage(float wage);
    void setCertification(const std::string& cert);

    // printable details 
    void printDetails(std::ostream& os) const;
};