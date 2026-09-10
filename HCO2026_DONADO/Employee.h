#pragma once
#include <string>
#include <iostream>
// Base Employee class
class Employee {
protected:
    int* employeeID;
    std::string* empFirstName;
    std::string* empLastName;
public:
    // three-argument constructor with default parameters
    Employee(int id = 0, const std::string& first = "", const std::string& last = "");
    // copy constructor (deep copy)
    Employee(const Employee& other);
    // assignment operator (deep copy)
    Employee& operator=(const Employee& other);
    virtual ~Employee();

    // accessors
    int getEmployeeID() const;
    std::string getFirstName() const;
    std::string getLastName() const;

    // mutators
    void setEmployeeID(int id);
    void setFirstName(const std::string& first);
    void setLastName(const std::string& last);

    // printable details; overridden in derived classes
    virtual void printDetails(std::ostream& os) const; // made virtual
};