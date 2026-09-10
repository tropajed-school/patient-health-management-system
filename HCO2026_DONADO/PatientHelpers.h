#pragma once

#include <vector>
#include <string>
#include "Patient.h"
#include "Service.h"
#include "Nurse.h"
#include "Aide.h"
#include "CaseManager.h"
#include "Equipment.h"
#include "StorageUnit.h"
#include "Mobility.h"
#include "Monitor.h"
#include "Exceptions.h"

// Service / patient helpers
std::vector<Service> loadServicesFromFile();
bool readPatientFromFile(const std::string& filename, Patient& outPatient, int patientID);
void savePatientToFile(const std::vector<Patient>& patients, const std::string& outFileName);
void displayAllPatients(const std::vector<Patient>& patients);
void displayPatient(const Patient& p);

// Patient list helpers
int findPatientByID(const std::vector<Patient>& patients, int patientID);
int findServiceByID(const std::vector<Service>& services, int serviceID);
void displayServices(const std::vector<Service>& services);
void sortPatientsByID(std::vector<Patient>& patients);
void showPatientList(const std::vector<Patient>& patients);

// Visit / case manager / employee helpers
void createVisitForPatient(Patient& patient, const std::vector<Service>& services,
const std::vector<Nurse>& nurses, const std::vector<Aide>& aides);
void assignCaseManagerToPatient(Patient& patient, const std::vector<CaseManager>& cms);
void printEmployeeReport(const std::vector<Nurse>& nurses, const std::vector<Aide>& aides, const std::vector<CaseManager>& cms);

// Equipment helpers
std::vector<Equipment*> loadEquipmentFromFile(const std::string& filename);
void printEquipmentList(const std::vector<Equipment*>& equipment);

// Single declaration only (implementation is in PatientHelpers.cpp)
void produceDepreciationSchedule(std::vector<Equipment*>& equipment);

// Small helpers
void pauseForEnter();
int generateUniquePatientID(const std::vector<Patient>& patients);

// Storage unit helpers
// Gather general information for a StorageUnit 
template<typename T>
void gatherStorageUnitInfo(StorageUnit<T>& unit) {
    int id = 0;
    std::string desc;
    int length = 0, width = 0, height = 0;
    double cost = 0.0;
    char ch = '\0';
    bool autoR = false;
    bool humidity = false;
    char dateBuf[32] = {0};

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter unit ID: ";
    std::cin >> id;
    std::cin.ignore(1000, '\n');

    std::cout << "Enter description: ";
    std::getline(std::cin, desc);

    // length
    bool gotLength = false;
    while (!gotLength) {
        try {
            std::cout << "Enter the length of the unit (positive integer): ";
            std::cin >> length;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); throw NegativeNumberException(); }
            unit.setLength(length);
            gotLength = true;
        } catch (const NegativeNumberException&) {
            std::cout << "Only positive numbers are allowed. Please re-enter.\n";
        }
    }

    // width
    bool gotWidth = false;
    while (!gotWidth) {
        try {
            std::cout << "Enter the width of the unit (positive integer): ";
            std::cin >> width;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); throw NegativeNumberException(); }
            unit.setWidth(width);
            gotWidth = true;
        } catch (const NegativeNumberException&) {
            std::cout << "Only positive numbers are allowed. Please re-enter.\n";
        }
    }

    // height
    bool gotHeight = false;
    while (!gotHeight) {
        try {
            std::cout << "Enter the height of the unit (positive integer): ";
            std::cin >> height;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); throw NegativeNumberException(); }
            unit.setHeight(height);
            gotHeight = true;
        } catch (const NegativeNumberException&) {
            std::cout << "Only positive numbers are allowed. Please re-enter.\n";
        }
    }

    // cost per month
    bool gotCost = false;
    while (!gotCost) {
        try {
            std::cout << "Enter the cost per month to rent the unit: ";
            std::cin >> cost;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000,'\n'); throw NegativeNumberException(); }
            unit.setCostPerMonth(cost);
            gotCost = true;
        } catch (const NegativeNumberException&) {
            std::cout << "Only positive numbers are allowed. Please re-enter.\n";
        }
    }

    std::cin.ignore(1000, '\n');
    std::cout << "Enter the date rented (MM/DD/YYYY): ";
    std::cin.getline(dateBuf, sizeof(dateBuf));

    // Parse date using Date (simple sscanf approach)
    int m = 0, d = 0, y = 0;
    if (sscanf_s(dateBuf, "%d/%d/%d", &m, &d, &y) == 3) {
        Date dt(m, d, y);
        unit.setDateRented(dt);
    } else {
        std::cout << "Warning: invalid date entered. Date left as default.\n";
    }

    // auto renew
    bool gotAuto = false;
    while (!gotAuto) {
        std::cout << "Does the contract auto-renew? (Y/N): ";
        std::cin >> ch;
        if (ch == 'Y' || ch == 'y') { autoR = true; gotAuto = true; }
        else if (ch == 'N' || ch == 'n') { autoR = false; gotAuto = true; }
        else { std::cout << "Invalid entry.\n"; }
    }
    unit.setAutoRenew(autoR);

    // humidity
    bool gotHumidity = false;
    while (!gotHumidity) {
        std::cout << "Is the unit humidity-controlled? (Y/N): ";
        std::cin >> ch;
        if (ch == 'Y' || ch == 'y') { humidity = true; gotHumidity = true; }
        else if (ch == 'N' || ch == 'n') { humidity = false; gotHumidity = true; }
        else { std::cout << "Invalid entry.\n"; }
    }
    unit.setHumidityControlled(humidity);

    unit.setUnitID(id);
    unit.setDescription(desc);

    std::cout << "\nStorage Unit updated successfully.\n";
    unit.printUnitInformation(std::cout);
    pauseForEnter();
}

// Print a storage unit information and inventory
template<typename T>
void viewStorageUnitInfo(StorageUnit<T>& unit) {
    unit.printUnitInformation(std::cout);
    unit.printInventory(std::cout);
}

// Add equipment to appropriate storage units (implementation in PatientHelpers.cpp)
void addEquipmentToStorageUnits(std::vector<Equipment*>& equipment,
                                StorageUnit<Mobility>& mobilityUnit,
                                StorageUnit<Monitor>& monitorUnit);
