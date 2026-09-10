#pragma once
#include "Employee.h"
#include <string>

class CaseManager : public Employee {
private:
    float salary;
    std::string degree;
public:
    // five-argument constructor with default parameters
    CaseManager(int id = 0, const std::string& first = "", const std::string& last = "",
                float sal = 0.0f, const std::string& deg = "");
    ~CaseManager();

    // accessors
    float getSalary() const;
    std::string getDegree() const;

    // mutators
    void setSalary(float sal);
    void setDegree(const std::string& deg);

    // printable details 
    void printDetails(std::ostream& os) const;
};