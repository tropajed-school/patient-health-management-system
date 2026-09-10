/*
Lab 8 Templates and Exception Handling Jedrik Donado 5/5/2026

Steps to accomplish in this lab:
Step 1: Housekeeping
- Ensure no functions are defined above main, only one return per function, and no returns in void functions.
Step 2: Exception handling
-Implement exception handling for 3 scenarios: Negative numbers, duplicate IDs, and invalid date format.
-This was put in a separate header to keep the main file cleaner and more focused on program flow.
Step 3: Patient h/cpp
-Add declarations to patient.h(visit getVisit and getNumVisits)
-Implment patient.cpp to return a copy of the Visit
Step 4: Modify Visit h/cpp
-Change to acccept only VisitID
-Replace tekenzieData implementation to throw Exception on error
Step 5: Enforce unique patient IDs
-Implement a helper function to generate unique patient IDs (1-1000) and use it when loading from file or adding manually. Throw an exception if no IDs are available.
Step 6: StorageUnit template
-Create a StorageUnit template class to manage storage of different equipment types.
-Only implement the header file, no cpp file
Step 7: Menu
-Show show equipment list, let user enter equipment ID, and add to appropriate storage unit based on type (Mobility or Monitor). Handle invalid IDs and type mismatches with exceptions.
*/
#include "Patient.h"
#include "Service.h"
#include "Nurse.h"
#include "Aide.h"
#include "CaseManager.h"
#include "helpers.h"
#include "PatientHelpers.h"    // moved helpers (non-template) here
#include "Equipment.h"
#include "StorageUnit.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <cctype>
#include <limits>

// Function prototypes - menus
void printMainMenu();
void printPatientsMenu();
void printEmployeesMenu();
void printEquipmentMenu();
int getMenuChoice(int minOpt, int maxOpt);

// Patient / service helpers 
// readPatientFromFile is declared in PatientHelpers.h; implementation remains here
bool readPatientFromFile(const std::string& filename, Patient& outPatient, int patientID);
void addPatientManually(std::vector<Patient>& patients);

// Prototype (implementation moved below main)
int generateUniquePatientID(const std::vector<Patient>& patients);

// Small helpers to remove repetitive code
// buildEmployeeVector template implemented in helpers.h

// Patient list helper (sorted by first name)
// showPatientList is declared/implemented in PatientHelpers.cpp

// Equipment helpers are declared/implemented in PatientHelpers.cpp

// Employee helpers declared/implemented in PatientHelpers.cpp

const int MAX_PATIENTS = 50;

int main() {
    std::vector<Patient> patients;
    std::vector<Service> services = loadServicesFromFile();
    std::vector<Equipment*> equipment = loadEquipmentFromFile("equipment.txt");

    // Create two StorageUnit objects at program start (one for Mobility, one for Monitor)
    StorageUnit<Mobility> mobilityUnit;
    StorageUnit<Monitor> monitorUnit;

    // Employee dynamic arrays (pointers to dynamically allocated objects)
    Nurse** nurses = nullptr;
    Aide** aides = nullptr;
    CaseManager** caseManagers = nullptr;
    int nurseCount = 0, aideCount = 0, cmCount = 0;

    // Load employees into dynamic arrays
    std::vector<Nurse> tempNurses;
    loadEmployeeData<Nurse>(tempNurses, "nurses.txt");
    nurseCount = (int)tempNurses.size();
    if (nurseCount > 0) {
        nurses = new Nurse*[nurseCount];
        for (int i = 0; i < nurseCount; ++i) nurses[i] = new Nurse(tempNurses[i]);
    }
    std::vector<Aide> tempAides;
    loadEmployeeData<Aide>(tempAides, "aides.txt");
    aideCount = (int)tempAides.size();
    if (aideCount > 0) {
        aides = new Aide*[aideCount];
        for (int i = 0; i < aideCount; ++i) aides[i] = new Aide(tempAides[i]);
    }
    std::vector<CaseManager> tempCms;
    loadEmployeeData<CaseManager>(tempCms, "caseManagers.txt");
    cmCount = (int)tempCms.size();
    if (cmCount > 0) {
        caseManagers = new CaseManager*[cmCount];
        for (int i = 0; i < cmCount; ++i) caseManagers[i] = new CaseManager(tempCms[i]);
    }

    // Main menu loop 
    int mainChoice = 0;
    do {
        printMainMenu();
        mainChoice = getMenuChoice(1, 4);

        if (mainChoice == 1) {
            // Patients submenu
            int pc = 0;
            do {
                printPatientsMenu();
                pc = getMenuChoice(1, 9);
                switch (pc) {
                case 1:
                    addPatientManually(patients);
                    break;
                case 2: {
                    if (patients.size() >= MAX_PATIENTS) {
                        std::cout << "Error: Health Care Facility is at capacity!\n";
                        pauseForEnter();
                        break;
                    }
                    std::string fileName;
                    std::cout << "Enter filename(ex. patient.txt): ";
                    std::cin.ignore(1000, '\n');
                    getline(std::cin, fileName);

                    int patientID = generateUniquePatientID(patients);
                    if (patientID == -1) {
                        std::cout << "Error: No available Patient IDs (1-1000). Cannot load new patient.\n";
                    } else {
                        Patient newPatient;
                        if (readPatientFromFile(fileName, newPatient, patientID)) {
                            patients.push_back(newPatient);
                            std::cout << "\nPatient data loaded successfully.\n";
                        } else {
                            std::cout << "\nFile could not be opened or read.\n";
                        }
                    }
                    pauseForEnter();
                    break;
                }
                case 3: {
                    if (patients.empty()) {
                        std::cout << "No patients to delete.\n\n";
                        pauseForEnter();
                    } else {
                        showPatientList(patients);
                        int patientID;
                        std::cout << "Enter Patient ID to delete: ";
                        std::cin >> patientID;
                        int idx = findPatientByID(patients, patientID);
                        if (idx == -1) {
                            std::cout << "Patient with ID " << patientID << " not found.\n";
                            pauseForEnter();
                        } else {
                            std::cout << "Patient found. Pre-deletion patient details:\n";
                            displayPatient(patients[idx]);

                            char confirm = '\0';
                            do {
                                std::cout << "Confirm deletion of this patient? (Y/N): ";
                                std::cin >> confirm;
                                if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); confirm = '\0'; }
                            } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

                            std::cin.ignore(1000, '\n');

                            if (confirm == 'Y' || confirm == 'y') {
                                patients.erase(patients.begin() + idx);
                                std::cout << "Patient deleted successfully.\n\n";
                                if (patients.empty()) {
                                    std::cout << "No patients remaining.\n";
                                    pauseForEnter();
                                } else {
                                    sortPatientsByID(patients);
                                    std::cout << "Updated patient list:\n";
                                    displayAllPatients(patients);
                                }
                            } else {
                                std::cout << "Deletion cancelled.\n";
                                pauseForEnter();
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    if (patients.empty()) { std::cout << "No patients available. Please add a patient first.\n\n"; break; }
                    if (services.empty()) { std::cout << "No services available. Cannot create a visit.\n\n"; break; }

                    showPatientList(patients);
                    int patientID;
                    std::cout << "Enter the ID of the patient to receive the visit: ";
                    std::cin >> patientID;

                    int patientIndex = findPatientByID(patients, patientID);
                    if (patientIndex == -1) {
                        std::cout << "Error: Patient with ID " << patientID << " not found.\n\n";
                    } else {
                        std::vector<Nurse> nurseVec = buildEmployeeVector<Nurse>(nurses, nurseCount);
                        std::vector<Aide> aideVec = buildEmployeeVector<Aide>(aides, aideCount);
                        createVisitForPatient(patients[patientIndex], services, nurseVec, aideVec);
                    }
                    break;
                }
                case 5: {
                    if (patients.empty()) { std::cout << "No patients available. Please add a patient first.\n\n"; break; }
                    if (cmCount == 0) { std::cout << "No case managers available.\n\n"; break; }

                    showPatientList(patients);
                    int patientID;
                    std::cout << "Enter the ID of the patient to assign a Case Manager: ";
                    std::cin >> patientID;
                    int pidx = findPatientByID(patients, patientID);
                    if (pidx == -1) {
                        std::cout << "Error: Patient with ID " << patientID << " not found.\n\n";
                    } else {
                        std::vector<CaseManager> cmVec = buildEmployeeVector<CaseManager>(caseManagers, cmCount);
                        assignCaseManagerToPatient(patients[pidx], cmVec);
                    }
                    break;
                }
                case 6:
                    sortPatientsByID(patients);
                    displayAllPatients(patients);
                    break;
                case 7: {
                    std::string outFileName;
                    std::cout << "Enter filename(ex. patient.txt): ";
                    std::cin.ignore(1000, '\n');
                    getline(std::cin, outFileName);
                    savePatientToFile(patients, outFileName);
                    break;
                }
                case 8: {
                    if (patients.empty()) { std::cout << "No patients available. Please add a patient first.\n\n"; break; }
                    showPatientList(patients);
                    int patientID;
                    std::cout << "Enter the ID of the patient to increase age: ";
                    std::cin >> patientID;
                    int pidx = findPatientByID(patients, patientID);
                    if (pidx == -1) {
                        std::cout << "Error: Patient with ID " << patientID << " not found.\n\n";
                    } else {
                        std::cout << "Selected patient (before change):\n";
                        std::cout << patients[pidx];
                        char confirm = '\0';
                        do {
                            std::cout << "Confirm increasing age by 1 for this patient? (Y/N): ";
                            std::cin >> confirm;
                            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(1000, '\n'); confirm = '\0'; }
                        } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');
                        std::cin.ignore(1000, '\n');
                        if (confirm == 'Y' || confirm == 'y') {
                            ++patients[pidx]; // uses overloaded ++
                            std::cout << "Patient age updated. New details:\n";
                            std::cout << patients[pidx];
                        } else {
                            std::cout << "Operation cancelled.\n";
                        }
                        pauseForEnter();
                    }
                    break;
                }
                case 9:
                    // return to main menu
                    break;
                }
            } while (pc != 9);
        }
        else if (mainChoice == 2) {
            // Employees submenu
            int ec = 0;
            do {
                printEmployeesMenu();
                ec = getMenuChoice(1, 2);
                switch (ec) {
                case 1: {
                    std::vector<Nurse> nurseVec = buildEmployeeVector<Nurse>(nurses, nurseCount);
                    std::vector<Aide> aideVec = buildEmployeeVector<Aide>(aides, aideCount);
                    std::vector<CaseManager> cmVec = buildEmployeeVector<CaseManager>(caseManagers, cmCount);
                    printEmployeeReport(nurseVec, aideVec, cmVec);
                    break;
                }
                case 2:
                    break; // return to main
                }
            } while (ec != 2);
        }
        else if (mainChoice == 3) {
            // Equipment submenu
            int eqc = 0;
            do {
                printEquipmentMenu();
                eqc = getMenuChoice(1, 6);
                switch (eqc) {
                case 1:
                    printEquipmentList(equipment);
                    pauseForEnter();
                    break;
                case 2:
                    produceDepreciationSchedule(equipment);
                    break;
                case 3: {
                    // Update storage unit information
                    int suChoice = -1;
                    do {
                        std::cout << "Enter 1 for the Mobility Unit, 2 for the Monitoring Unit, 0 to exit: ";
                        suChoice = getMenuChoice(0, 2);
                        if (suChoice == 1) {
                            try {
                                gatherStorageUnitInfo(mobilityUnit);
                            } catch (const NegativeNumberException& ex) {
                                std::cout << ex.what() << "\n";
                                pauseForEnter();
                            }
                        } else if (suChoice == 2) {
                            try {
                                gatherStorageUnitInfo(monitorUnit);
                            } catch (const NegativeNumberException& ex) {
                                std::cout << ex.what() << "\n";
                                pauseForEnter();
                            }
                        }
                    } while (suChoice != 0);
                    break;
                }
                case 4:
                    // Add equipment to storage unit
                    addEquipmentToStorageUnits(equipment, mobilityUnit, monitorUnit);
                    break;
                case 5: {
                    // View storage unit information
                    int suChoice = -1;
                    do {
                        std::cout << "Enter 1 for the Mobility Unit, 2 for the Monitoring Unit, 0 to exit: ";
                        suChoice = getMenuChoice(0, 2);
                        if (suChoice == 1) {
                            viewStorageUnitInfo(mobilityUnit);
                            pauseForEnter();
                        } else if (suChoice == 2) {
                            viewStorageUnitInfo(monitorUnit);
                            pauseForEnter();
                        }
                    } while (suChoice != 0);
                    break;
                }
                case 6:
                    break; // return to main
                }
            } while (eqc != 6);
        }
    } while (mainChoice != 4);

    // Cleanup (employees)
    if (nurses) {
        for (int i = 0; i < nurseCount; ++i) { delete nurses[i]; nurses[i] = nullptr; }
        delete[] nurses; nurses = nullptr;
    }
    if (aides) {
        for (int i = 0; i < aideCount; ++i) { delete aides[i]; aides[i] = nullptr; }
        delete[] aides; aides = nullptr;
    }
    if (caseManagers) {
        for (int i = 0; i < cmCount; ++i) { delete caseManagers[i]; caseManagers[i] = nullptr; }
        delete[] caseManagers; caseManagers = nullptr;
    }

    // Clean up equipment
    for (int i = 0; i < static_cast<int>(equipment.size()); ++i) {
        delete equipment[i];
    }
    equipment.clear();

    return 0;
}


void printMainMenu() {
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                     Health Care Options, Inc.\n";
    std::cout << "                       Patient Health Management\n";
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "      1.  Patients\n";
    std::cout << "      2.  Employees\n";
    std::cout << "      3.  Equipment\n";
    std::cout << "      4.  Exit\n\n";
}

void printPatientsMenu() {
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                     Health Care Options, Inc.\n";
    std::cout << "                          Patient Management\n";
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "      1.  Add new patient manually\n";
    std::cout << "      2.  Read patient information from file\n";
    std::cout << "      3.  Delete existing patient\n";
    std::cout << "      4.  Create visit for patient\n";
    std::cout << "      5.  Assign case manager to patient\n";
    std::cout << "      6.  Print patient report\n";
    std::cout << "      7.  Save patient report\n";
    std::cout << "      8.  Increase patient age\n";
    std::cout << "      9.  Return to Main Menu\n\n";
}

void printEmployeesMenu() {
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                     Health Care Options, Inc.\n";
    std::cout << "                          Employee Management\n";
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "      1.  Print employee report\n";
    std::cout << "      2.  Return to Main Menu\n\n";
}

void printEquipmentMenu() {
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                     Health Care Options, Inc.\n";
    std::cout << "                          Equipment Management\n";
    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "      1.  Print equipment list\n";
    std::cout << "      2.  Produce depreciation schedule\n";
    std::cout << "      3.  Update storage unit information\n";
    std::cout << "      4.  Add equipment to storage unit\n";
    std::cout << "      5.  View storage unit information\n";
    std::cout << "      6.  Return to Main Menu\n\n";
}

int getMenuChoice(int minOpt, int maxOpt) {
    int choice;
    bool validInput = false;
    do {
        std::cout << "Enter selection: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Error: Please enter a valid number (" << minOpt << "-" << maxOpt << ").\n";
        } else if (choice < minOpt || choice > maxOpt) {
            std::cout << "Error: Please enter a number between " << minOpt << " and " << maxOpt << ".\n";
        } else {
            validInput = true;
        }
    } while (!validInput);
    return choice;
}

// readPatientFromFile implementation remains in this translation unit
bool readPatientFromFile(const std::string& filename, Patient& outPatient, int patientID) {
    bool success = true;
    std::ifstream in(filename);
    if (!in.is_open()) {
        success = false;
    } else {
        std::string first, last, address;
        std::string line;

        if (!std::getline(in, first)) { success = false; }
        if (success && !std::getline(in, last)) { success = false; }
        if (success && !std::getline(in, address)) { success = false; }

        int age = 0;
        float height = 0.0f;
        float weight = 0.0f;
        char gender = ' ';
        bool ambulatory = false;

        if (success) {
            // age
            if (std::getline(in, line)) {
                std::istringstream ss(line);
                ss >> age;
            }
            // height
            if (std::getline(in, line)) {
                std::istringstream ss(line);
                ss >> height;
            }
            // weight
            if (std::getline(in, line)) {
                std::istringstream ss(line);
                ss >> weight;
            }
            // gender
            if (std::getline(in, line)) {
                for (char c : line) { if (!std::isspace(static_cast<unsigned char>(c))) { gender = c; break; } }
            }
            // ambulatory
            if (std::getline(in, line)) {
                char c = 'N';
                for (char ch : line) { if (!std::isspace(static_cast<unsigned char>(ch))) { c = ch; break; } }
                ambulatory = (c == 'Y' || c == 'y');
            }

            // construct patient
            outPatient = Patient(patientID, first, last, address, age, height, weight, gender, ambulatory);

            // optional bpm readings
            if (std::getline(in, line)) {
                int n = 0;
                std::istringstream ss(line);
                if (ss >> n && n > 0) {
                    if (std::getline(in, line)) {
                        std::istringstream ss2(line);
                        int bpm;
                        int count = 0;
                        while (ss2 >> bpm && count < n) {
                            outPatient.addBpmReading(bpm);
                            ++count;
                        }
                    }
                }
            }
        }
        in.close();
    }
    return success;
}

// addPatientManually remains in this file 
void addPatientManually(std::vector<Patient>& patients) {
    if (patients.size() >= static_cast<size_t>(MAX_PATIENTS)) {
        std::cout << "Error: Health Care Facility is at capacity!\n";
        pauseForEnter();
    } else {
        int id = generateUniquePatientID(patients);
        if (id == -1) {
            std::cout << "Error: No available Patient IDs (1-1000). Cannot add new patient.\n";
            pauseForEnter();
        } else {
            std::string firstName, lastName, address;
            int age = 0;
            float height = 0.0f, weight = 0.0f;
            char gender = ' ';
            char ambChar = 'N';
            bool isAmb = false;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter first name: ";
            std::getline(std::cin, firstName);
            while (firstName.empty()) {
                std::cout << "First name cannot be empty. Enter first name: ";
                std::getline(std::cin, firstName);
            }

            std::cout << "Enter last name: ";
            std::getline(std::cin, lastName);
            while (lastName.empty()) {
                std::cout << "Last name cannot be empty. Enter last name: ";
                std::getline(std::cin, lastName);
            }

            std::cout << "Enter street address: ";
            std::getline(std::cin, address);
            while (address.empty()) {
                std::cout << "Street address cannot be empty. Enter street address: ";
                std::getline(std::cin, address);
            }

            bool valid = false;
            do {
                std::cout << "Enter age (1-120): ";
                std::cin >> age;
                if (!std::cin.fail() && age >= 1 && age <= 120) valid = true;
                else {
                    std::cout << "Invalid age.\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                }
            } while (!valid);

            valid = false;
            do {
                std::cout << "Enter height in inches (12-96): ";
                std::cin >> height;
                if (!std::cin.fail() && height >= 12.0f && height <= 96.0f) valid = true;
                else {
                    std::cout << "Invalid height.\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                }
            } while (!valid);

            valid = false;
            do {
                std::cout << "Enter weight in pounds (1-1000): ";
                std::cin >> weight;
                if (!std::cin.fail() && weight >= 1.0f && weight <= 1000.0f) valid = true;
                else {
                    std::cout << "Invalid weight.\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                }
            } while (!valid);

            std::cin.clear();
            std::cin.ignore(1000, '\n');

            // Controlled loop for gender 
            bool gotGender = false;
            while (!gotGender) {
                std::cout << "Enter gender (M/F): ";
                std::string gline;
                std::getline(std::cin, gline);
                if (!gline.empty()) {
                    char c = gline[0];
                    if (c == 'm' || c == 'M' || c == 'f' || c == 'F') {
                        gender = std::toupper(static_cast<unsigned char>(c));
                        gotGender = true;
                    }
                }
                if (!gotGender) std::cout << "Invalid gender. ";
            }

            // Controlled loop for ambulatory 
            bool gotAmb = false;
            while (!gotAmb) {
                std::cout << "Is the patient ambulatory? (Y/N): ";
                std::string aline;
                std::getline(std::cin, aline);
                if (!aline.empty()) {
                    char c = aline[0];
                    if (c == 'Y' || c == 'y' || c == 'N' || c == 'n') {
                        ambChar = c;
                        gotAmb = true;
                    }
                }
                if (!gotAmb) std::cout << "Invalid entry. ";
            }
            isAmb = (ambChar == 'Y' || ambChar == 'y');

            Patient newP(id, firstName, lastName, address, age, height, weight, gender, isAmb);
            patients.push_back(newP);

            std::cout << "\nPatient added successfully:\n";
            std::cout << newP;
            pauseForEnter();
        }
    }
}
// generateUniquePatientID implementation(below main)
int generateUniquePatientID(const std::vector<Patient>& patients) {
    const int MIN_ID = 1;
    const int MAX_ID = 1000;

    int result = -1;

    if (patients.size() < static_cast<size_t>(MAX_ID)) {
        std::vector<bool> used(MAX_ID + 1, false);
        for (const Patient& p : patients) {
            int id = p.getPatientID();
            if (id >= MIN_ID && id <= MAX_ID) used[id] = true;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(MIN_ID, MAX_ID);

        // Try random picks first 
        for (int attempt = 0; attempt < 2000 && result == -1; ++attempt) {
            int cand = dist(gen);
            if (!used[cand]) result = cand;
        }

        // Fallback to linear scan if needed
        if (result == -1) {
            for (int cand = MIN_ID; cand <= MAX_ID; ++cand) {
                if (!used[cand]) { result = cand; break; }
            }
        }
    }

    return result;
}