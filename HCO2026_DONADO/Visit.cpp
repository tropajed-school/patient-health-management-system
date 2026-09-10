#include "Visit.h"
#include "Exceptions.h"
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cctype>

// Constructor
Visit::Visit(int id, const std::string&, const std::string& date)
    : visitID(id), visitDate(), nurse(), aide() {
   
    if (!date.empty()) {
        setVisitDate(date);
    }
}

// Destructor
Visit::~Visit() {}

// This implementation validates format and throws InvalidDateException on errors.
void Visit::tokenizeDate(char* dateStr, int& month, int& day, int& year) {
    if (dateStr == nullptr || std::strlen(dateStr) == 0) {
        throw InvalidDateException();
    }
    int len = static_cast<int>(std::strlen(dateStr));
    if (len < 8 || len > 10) { // m/d/yyyy (8) to mm/dd/yyyy (10)
        throw InvalidDateException();
    }

    int i = 0;
    int digitCount = 0;
    // Parse month
    month = 0;
    while (dateStr[i] != '/' && dateStr[i] != '\0') {
        if (!std::isdigit(static_cast<unsigned char>(dateStr[i]))) throw InvalidDateException();
        month = month * 10 + (dateStr[i] - '0');
        ++i;
    }
    if (dateStr[i] != '/') throw InvalidDateException();
    ++i; // skip '/'

    // Parse day
    day = 0;
    while (dateStr[i] != '/' && dateStr[i] != '\0') {
        if (!std::isdigit(static_cast<unsigned char>(dateStr[i]))) throw InvalidDateException();
        day = day * 10 + (dateStr[i] - '0');
        ++i;
    }
    if (dateStr[i] != '/') throw InvalidDateException();
    ++i; // skip '/'

    // Parse year
    year = 0;
    digitCount = 0;
    while (dateStr[i] != '\0') {
        if (!std::isdigit(static_cast<unsigned char>(dateStr[i]))) throw InvalidDateException();
        year = year * 10 + (dateStr[i] - '0');
        ++digitCount;
        ++i;
    }
    if (digitCount != 4) throw InvalidDateException();

    // Validate month/day/year ranges
    if (month < 1 || month > 12) throw InvalidDateException();

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    // Leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1]) throw InvalidDateException();
}

// Accessors
int Visit::getVisitID() const { return visitID; }

std::string Visit::getEmployeeName() const {
    std::string name;
    if (nurse.getEmployeeID() != 0) {
        name = nurse.getFirstName() + " " + nurse.getLastName();
    }
    if (aide.getEmployeeID() != 0) {
        if (!name.empty()) name += " & ";
        name += aide.getFirstName() + " " + aide.getLastName();
    }
    return name;
}

// Build formatted date using Date::toString()
std::string Visit::getVisitDate() const {
    return visitDate.toString();
}

// Mutators
void Visit::setVisitID(int id) { visitID = id; }

// setVisitDate from character array (tokenize then set Date)
void Visit::setVisitDate(const char* dateChar) {
    if (dateChar) {
        // make a mutable copy for tokenizer
        char buffer[64] = {0};
        size_t srcLen = std::strlen(dateChar);
        size_t copyLen = (srcLen < (sizeof(buffer) - 1)) ? srcLen : (sizeof(buffer) - 1);
        if (copyLen > 0) std::memcpy(buffer, dateChar, copyLen);
        buffer[copyLen] = '\0';
        int m = 0, d = 0, y = 0;
        try {
            tokenizeDate(buffer, m, d, y);
            visitDate.setDate(m, d, y);
        } catch (const InvalidDateException&) {
            // Re-throw to let the caller handle 
            throw;
        }
    }
}

// string overload
void Visit::setVisitDate(const std::string& dateStr) {
    char buffer[64] = {0};
    size_t srcLen = dateStr.size();
    size_t copyLen = (srcLen < (sizeof(buffer) - 1)) ? srcLen : (sizeof(buffer) - 1);
    if (copyLen > 0) std::memcpy(buffer, dateStr.c_str(), copyLen);
    buffer[copyLen] = '\0';
    int m = 0, d = 0, y = 0;
    // let tokenizeDate throw InvalidDateException on errors
    tokenizeDate(buffer, m, d, y);
    visitDate.setDate(m, d, y);
}

void Visit::setNurse(const Nurse& n) { nurse = n; }
const Nurse& Visit::getNurse() const { return nurse; }
void Visit::setAide(const Aide& a) { aide = a; }
const Aide& Visit::getAide() const { return aide; }

// Add a service to this visit
void Visit::addService(const Service& service) {
    services.push_back(service);
}

// Print all visit details including services and employee names
void Visit::printVisit(std::ostream& os) const {
    os << std::left << std::setw(17) << "Visit ID:" << getVisitID() << "\n";
    os << std::left << std::setw(17) << "Visit Date:" << getVisitDate() << "\n";

    // Print nurse/aide names 
    if (nurse.getEmployeeID() != 0) {
        os << std::left << std::setw(17) << "Nurse:" << nurse.getFirstName() << " " << nurse.getLastName() << "\n";
    } else {
        os << std::left << std::setw(17) << "Nurse:" << "None assigned" << "\n";
    }

    if (aide.getEmployeeID() != 0) {
        os << std::left << std::setw(17) << "Aide:" << aide.getFirstName() << " " << aide.getLastName() << "\n";
    } else {
        os << std::left << std::setw(17) << "Aide:" << "None assigned" << "\n";
    }

    if (!services.empty()) {
        os << "Services provided during this visit:\n";
        for (int i = 0; i < static_cast<int>(services.size()); i++) {
            os << "    " << services[i].getServiceID() << " - " << services[i].getServiceName() << "\n";
        }
    }
    os << "\n";
}