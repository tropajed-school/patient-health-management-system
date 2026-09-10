#include "Employee.h"
#include <iomanip>

// Constructor: allocate and initialize members
Employee::Employee(int id, const std::string& first, const std::string& last) {
    employeeID = new int(id);
    empFirstName = new std::string(first);
    empLastName = new std::string(last);
}

// Copy constructor (deep copy)
Employee::Employee(const Employee& other) {
    if (other.employeeID)
        employeeID = new int(*other.employeeID);
    else
        employeeID = new int(0);

    if (other.empFirstName)
        empFirstName = new std::string(*other.empFirstName);
    else
        empFirstName = new std::string("");

    if (other.empLastName)
        empLastName = new std::string(*other.empLastName);
    else
        empLastName = new std::string("");
}

// Assignment operator (deep copy)
Employee& Employee::operator=(const Employee& other) {
    if (this != &other) {
        // employeeID
        if (employeeID) { delete employeeID; employeeID = nullptr; }
        if (other.employeeID) employeeID = new int(*other.employeeID);
        else employeeID = new int(0);

        // empFirstName
        if (empFirstName) { delete empFirstName; empFirstName = nullptr; }
        if (other.empFirstName) empFirstName = new std::string(*other.empFirstName);
        else empFirstName = new std::string("");

        // empLastName
        if (empLastName) { delete empLastName; empLastName = nullptr; }
        if (other.empLastName) empLastName = new std::string(*other.empLastName);
        else empLastName = new std::string("");
    }
    return *this;
}

// Destructor: free allocated memory
Employee::~Employee() {
    if (employeeID) { delete employeeID; employeeID = nullptr; }
    if (empFirstName) { delete empFirstName; empFirstName = nullptr; }
    if (empLastName) { delete empLastName; empLastName = nullptr; }
}

// Accessors
int Employee::getEmployeeID() const { return (employeeID ? *employeeID : 0); }
std::string Employee::getFirstName() const { return (empFirstName ? *empFirstName : std::string()); }
std::string Employee::getLastName() const { return (empLastName ? *empLastName : std::string()); }

// Mutators
void Employee::setEmployeeID(int id) {
    if (!employeeID) employeeID = new int;
    *employeeID = id;
}
void Employee::setFirstName(const std::string& first) {
    if (!empFirstName) empFirstName = new std::string;
    *empFirstName = first;
}
void Employee::setLastName(const std::string& last) {
    if (!empLastName) empLastName = new std::string;
    *empLastName = last;
}

void Employee::printDetails(std::ostream& os) const {
    os << std::left << std::setw(5) << getEmployeeID() << " "
       << getFirstName() << " " << getLastName();
}