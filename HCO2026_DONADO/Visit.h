#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Service.h"
#include "Nurse.h"
#include "Aide.h"
#include "Date.h"

class Visit {
private:
    int visitID;
    Date visitDate;
    std::vector<Service> services;  // A visit can have multiple services
    Nurse nurse; 
    Aide aide;  

    // tokenizes date char array into month/day/year 
    static void tokenizeDate(char* c, int& month, int& day, int& year);
//
public:
    Visit(int id = 0, const std::string& empName = "", const std::string& date = "");

    ~Visit();

    // Accessors
    int getVisitID() const;
	// getEmployeeName returns a combined string of nurse and aide names (if assigned)
    std::string getEmployeeName() const;
    std::string getVisitDate() const;

    // Mutators
    void setVisitID(int id);
    // Now accept character array per requirements
    void setVisitDate(const char* dateChar);
    void setVisitDate(const std::string& dateStr);

    // Nurse/Aide accessors and mutators
    void setNurse(const Nurse& n);
    const Nurse& getNurse() const;
    void setAide(const Aide& a);
    const Aide& getAide() const;

    // Function to add a service to this visit
    void addService(const Service& service);

    // Function to print visit details
    void printVisit(std::ostream& os) const;
};