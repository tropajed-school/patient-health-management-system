#include "PatientHelpers.h"
#include "helpers.h"
#include "Visit.h"
#include "Date.h"
#include "Monitor.h"
#include "Mobility.h"
#include "Exceptions.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <typeinfo>

// Load services from file
std::vector<Service> loadServicesFromFile() {
    std::vector<Service> services;
    std::ifstream in("services.txt");
    bool opened = in.is_open();
    if (!opened) {
        std::cout << "Warning: Could not open services.txt file. No services available.\n";
    } else {
        int serviceID;
        std::string serviceName;
        while (in >> serviceID) {
            in.ignore(); // Skip newline after serviceID
            if (!std::getline(in, serviceName)) break;
            services.emplace_back(serviceID, serviceName);
        }
        in.close();
        if (!services.empty()) std::cout << "Services loaded successfully from services.txt\n";
    }
    return services;
}

// Small helpers
void pauseForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Display services
void displayServices(const std::vector<Service>& services) {
    std::cout << "\nThe following services are currently offered by HCO:\n";
    int n = static_cast<int>(services.size());
    for (int i = 0; i < n; ++i) {
        std::cout << services[i].getServiceID() << "\t\t" << services[i].getServiceName() << "\n";
    }
    std::cout << "\n";
}

// Find helpers
int findPatientByID(const std::vector<Patient>& patients, int patientID) {
    int result = -1;
    int n = static_cast<int>(patients.size());
    for (int i = 0; i < n; ++i) {
        if (patients[i].getPatientID() == patientID) { result = i; break; }
    }
    return result;
}

int findServiceByID(const std::vector<Service>& services, int serviceID) {
    int result = -1;
    int n = static_cast<int>(services.size());
    for (int i = 0; i < n; ++i) {
        if (services[i].getServiceID() == serviceID) { result = i; break; }
    }
    return result;
}

// Display patient(s)
void displayPatient(const Patient& p) {
    std::cout << p;
}
// Display all patients sorted by ID (make a copy to sort so original order is not changed)
void displayAllPatients(const std::vector<Patient>& patients) {
    if (patients.empty()) {
        std::cout << "No patients to display.\n\n";
    } else {
        std::vector<Patient> copy = patients;
        std::sort(copy.begin(), copy.end(), [](const Patient& a, const Patient& b){
            return a.getPatientID() < b.getPatientID();
        });
        std::cout << "---------------------------------------------------------------------\n";
        std::cout << "                       Patient Report\n";
        std::cout << "---------------------------------------------------------------------\n";
        for (int i = 0; i < static_cast<int>(copy.size()); ++i) {
            const Patient& p = copy[i];
            std::cout << p;
        }
        std::cout << "---------------------------------------------------------------------\n";
    }
}
// Save patient report to file (sorted by ID)
void savePatientToFile(const std::vector<Patient>& patients, const std::string& outFileName) {
    std::ofstream out(outFileName);
    bool opened = out.is_open();
    if (!opened) {
        std::cout << "Error: Could not open " << outFileName << " for writing.\n";
    } else {
        out << "---------------------------------------------------------------------\n";
        out << "                       Patient Report\n";
        out << "---------------------------------------------------------------------\n";
        for (int i = 0; i < static_cast<int>(patients.size()); ++i) {
            const Patient& p = patients[i];
            out << p;
        }
        out << "---------------------------------------------------------------------\n";
        out.close();
        std::cout << "Patient report saved to " << outFileName << "\n";
    }
}

// Visit creation
void createVisitForPatient(Patient& patient, const std::vector<Service>& services,
                           const std::vector<Nurse>& nurses, const std::vector<Aide>& aides) {
    std::string message = "";

    int visitID = 0;
    char visitDateChar[64] = {0};
    bool saveVisit = true;

    // Prompt for Visit ID and ensure it's positive and unique for this patient
    while (true) {
        std::cout << "Enter Visit ID: ";
        std::cin >> visitID;
        if (std::cin.fail() || visitID <= 0) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Error: Please enter a positive integer for Visit ID.\n";
            continue;
        }

        // check uniqueness for this patient; throw/catch DuplicateIDException when duplicate
        try {
            for (int i = 0; i < patient.getNumVisits(); ++i) {
                if (patient.getVisit(i).getVisitID() == visitID) {
                    throw DuplicateIDException();
                }
            }
            break; // unique -> exit loop
        } catch (const DuplicateIDException& ex) {
            std::cout << ex.what() << " Please enter a different Visit ID.\n";
            // continue loop to re-prompt
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
    }

    std::cin.ignore(1000, '\n');

    // Visit date 
    while (true) {
        std::cout << "Visit date (mm/dd/yyyy): ";
        std::cin.getline(visitDateChar, sizeof(visitDateChar));
        if (visitDateChar[0] == '\0') {
            std::cout << "Error: Visit date cannot be empty.\n";
            continue;
        }
        // attempt to set date; setVisitDate may throw InvalidDateException
        Visit newVisit(visitID);
        try {
            newVisit.setVisitDate(visitDateChar);
            // if successful, continue filling out visit below
            // Select Nurse (0 for none)
            if (!nurses.empty()) {
                showEmployees<Nurse>(nurses, "The following Nurses are currently employed by HCO :");
                std::cout << "Enter the ID of the Nurse to assign (enter 0 if none): ";
                int nid = 0;
                std::cin >> nid;
                if (!std::cin.fail() && nid != 0) {
                    int idx = findEmployee<Nurse>(nid, nurses);
                    if (idx != -1) {
                        newVisit.setNurse(nurses[idx]);
                        std::cout << "Nurse assigned.\n";
                    } else {
                        std::cout << "Invalid Nurse ID entered. No nurse assigned.\n";
                    }
                }
            }

            // Select Aide (0 for none)
            if (!aides.empty()) {
                showEmployees<Aide>(aides, "The following Aides are currently employed by HCO :");
                std::cout << "Enter the ID of the Aide to assign (enter 0 if none): ";
                int aid = 0;
                std::cin >> aid;
                if (!std::cin.fail() && aid != 0) {
                    int idx = findEmployee<Aide>(aid, aides);
                    if (idx != -1) {
                        newVisit.setAide(aides[idx]);
                        std::cout << "Aide assigned.\n";
                    } else {
                        std::cout << "Invalid Aide ID entered. No aide assigned.\n";
                    }
                }
            }

            // Add services to the visit
            displayServices(services);

            int serviceID = -1;
            int addedCount = 0;
            do {
                std::cout << "Enter the ID of the Service to add (enter 0 to quit): ";
                std::cin >> serviceID;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Error: Please enter a valid numeric service ID.\n";
                    serviceID = -1; // loop again
                    continue;
                }

                if (serviceID == 0) break;

                int serviceIndex = findServiceByID(services, serviceID);
                if (serviceIndex != -1) {
                    newVisit.addService(services[serviceIndex]);
                    ++addedCount;
                    std::cout << "Service added successfully.\n";
                } else {
                    std::cout << "Error: Service with ID " << serviceID << " not found. Please try again.\n";
                }
            } while (serviceID != 0);

            if (addedCount == 0) {
                char ch = '\0';
                do {
                    std::cout << "No services added to this visit. Do you want to save an empty visit? (Y/N): ";
                    std::cin >> ch;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        ch = '\0';
                    }
                } while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n');

                if (ch == 'N' || ch == 'n') saveVisit = false;
            }
            
            // Finally, add the visit to the patient if confirmed
            if (saveVisit) {
                patient.addVisit(newVisit);
                message = "Visit created and added to patient successfully.";
            } else {
                message = "Visit discarded.\n";
            }
            if (!message.empty()) std::cout << message << "\n";
            pauseForEnter();
            break; // done with visit creation
        } catch (const InvalidDateException& ex) {
            std::cout << ex.what() << " Please re-enter a valid date in the format mm/dd/yyyy.\n";
            // loop and re-prompt date
        }
    }
}

// Assign case manager to patient
void assignCaseManagerToPatient(Patient& patient, const std::vector<CaseManager>& cms) {
    if (cms.empty()) {
        std::cout << "No case managers available.\n";
        pauseForEnter();
    } else {
        showEmployees<CaseManager>(cms, "The following Case Managers are currently employed by HCO :");
        std::cout << "Enter the ID of the Case Manager to assign (enter 0 to clear assignment): ";
        int cmid = -1;
        std::cin >> cmid;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid entry.\n";
            pauseForEnter();
        } else {
            if (cmid == 0) {
                patient.setCaseManager(CaseManager()); // default / cleared
                std::cout << "Case Manager cleared for patient.\n";
                pauseForEnter();
            } else {
                int cidx = findEmployee<CaseManager>(cmid, cms);
                if (cidx == -1) {
                    std::cout << "Case Manager ID not found.\n";
                } else {
                    patient.setCaseManager(cms[cidx]);
                    std::cout << "Case Manager assigned to patient.\n";
                }
                pauseForEnter();
            }
        }
    }
}

// Print employee report
void printEmployeeReport(const std::vector<Nurse>& nursesIn, const std::vector<Aide>& aidesIn, const std::vector<CaseManager>& cmsIn) {
    std::vector<Nurse> nurses = nursesIn;
    std::vector<Aide> aides = aidesIn;
    std::vector<CaseManager> cms = cmsIn;

    sortEmployeesByLastName(nurses);
    sortEmployeesByLastName(aides);
    sortEmployeesByLastName(cms);

    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                     Health Care Options, Inc.\n";
    std::cout << "                            Employee List\n";
    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Nurses:\n\n";
    std::cout << std::left << std::setw(5) << "ID" << "  " << std::setw(20) << "Employee Name" << "  " << std::setw(12) << "Hourly Wage" << "  " << "Designation\n";
    std::cout << "---------------------------------------------------------------------\n";
    int nN = (int)nurses.size();
    for (int i = 0; i < nN; ++i) {
        const Nurse& n = nurses[i];
        n.printDetails(std::cout);
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "Aides:\n\n";
    std::cout << std::left << std::setw(5) << "ID" << "  " << std::setw(20) << "Employee Name" << "  " << std::setw(12) << "Hourly Wage" << "  " << "Certification\n";
    std::cout << "---------------------------------------------------------------------\n";
    int nA = (int)aides.size();
    for (int i = 0; i < nA; ++i) {
        const Aide& a = aides[i];
        a.printDetails(std::cout);
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "Case Managers:\n\n";
    std::cout << std::left << std::setw(5) << "ID" << "  " << std::setw(20) << "Employee Name" << "  " << std::setw(12) << "Salary" << "  " << "Degree\n";
    std::cout << "---------------------------------------------------------------------\n";
    int nC = (int)cms.size();
    for (int i = 0; i < nC; ++i) {
        const CaseManager& c = cms[i];
        c.printDetails(std::cout);
        std::cout << "\n";
    }

    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "                            End of Report\n";
    std::cout << "---------------------------------------------------------------------\n";
    pauseForEnter();
}

// Equipment helpers
std::vector<Equipment*> loadEquipmentFromFile(const std::string& filename) {
    std::vector<Equipment*> result;
    std::ifstream in(filename);
    bool opened = in.is_open();
    if (!opened) {
        std::cout << "Warning: Could not open " << filename << ". No equipment loaded.\n";
    } else {
        std::string line;
        for (;;) {
            // Read ID line (skip blank lines)
            std::string idLine;
            bool gotIdLine = false;
            while (true) {
                if (!std::getline(in, idLine)) { gotIdLine = false; break; }
                if (!idLine.empty()) { gotIdLine = true; break; }
            }
            if (!gotIdLine) break;

            std::istringstream idss(idLine);
            int id = 0;
            if (!(idss >> id)) {
                // malformed entry -- stop processing
                break;
            }

            // Read remaining 6 lines for the record
            std::string name;
            if (!std::getline(in, name)) break;

            std::string dateLine;
            if (!std::getline(in, dateLine)) break;

            std::string costLine;
            if (!std::getline(in, costLine)) break;
            std::istringstream costss(costLine);
            double cost = 0.0;
            if (!(costss >> cost)) break;

            std::string lifeLine;
            if (!std::getline(in, lifeLine)) break;
            std::istringstream lifess(lifeLine);
            int life = 0;
            if (!(lifess >> life)) break;

            std::string salvageLine;
            if (!std::getline(in, salvageLine)) break;
            std::istringstream salvagess(salvageLine);
            double salvage = 0.0;
            if (!(salvagess >> salvage)) break;

            std::string extra;
            if (!std::getline(in, extra)) extra = "";

            // Parse date (mm/dd/yyyy)
            int m = 0, d = 0, y = 0;
            if (sscanf_s(dateLine.c_str(), "%d/%d/%d", &m, &d, &y) != 3) { m = d = y = 0; }
            Date dt(m, d, y);

            // Decide derived type by useful life (<=5 -> Monitor, >5 -> Mobility)
            if (life <= 5) {
                result.push_back(new Monitor(id, name, dt, cost, life, salvage, extra));
            } else {
                result.push_back(new Mobility(id, name, dt, cost, life, salvage, extra));
            }
        }
        in.close();
        if (!result.empty()) std::cout << "Equipment loaded from " << filename << "\n";
    }
    return result;
}

// Print equipment list sorted by ID
void printEquipmentList(const std::vector<Equipment*>& equipment) {
    if (equipment.empty()) {
        std::cout << "No equipment to display.\n";
    } else {
        std::vector<Equipment*> copy = equipment;
        std::sort(copy.begin(), copy.end(), [](const Equipment* a, const Equipment* b) {
            return *a < *b;
        });

        std::cout << "---------------------------------------------------------------------\n";
        std::cout << "                     Health Care Options, Inc.\n";
        std::cout << "                            Equipment List\n";
        std::cout << "---------------------------------------------------------------------\n";
        std::cout << "ID   Name                 Date       Cost       Life Salvage  Mode/Display\n";
        std::cout << "---------------------------------------------------------------------\n";
        for (int i = 0; i < static_cast<int>(copy.size()); ++i) {
            const Equipment* e = copy[i];
            e->printDetails(std::cout);
        }
        std::cout << "---------------------------------------------------------------------\n";
    }
}

// Prompt user to select equipment by ID and produce depreciation schedule
void produceDepreciationSchedule(std::vector<Equipment*>& equipment) {
    if (equipment.empty()) {
        std::cout << "No equipment available.\n";
        pauseForEnter();
    } else {
        printEquipmentList(equipment);
        std::cout << "Enter Equipment ID to produce depreciation schedule: ";
        int id = 0;
        std::cin >> id;
        bool invalidInput = false;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid entry.\n";
            invalidInput = true;
        }

        if (invalidInput) {
            pauseForEnter();
        } else {
            Equipment* found = nullptr;
            for (int i = 0; i < static_cast<int>(equipment.size()); ++i) {
                Equipment* e = equipment[i];
                if (e->getEquipmentID() == id) { found = e; break; }
            }
            if (!found) {
                std::cout << "Equipment with ID " << id << " not found.\n";
                pauseForEnter();
            } else {
                found->depreciate(std::cout);
                pauseForEnter();
            }
        }
    }
}

// Patient list helper
void showPatientList(const std::vector<Patient>& patients) {
    if (patients.empty()) {
        std::cout << "No patients available.\n\n";
        pauseForEnter();
    } else {
        std::vector<Patient> copy = patients;
        std::sort(copy.begin(), copy.end()); // uses Patient::operator<
        std::cout << "\nPatient List (ID  Name) - sorted by first name:\n";
        for (int i = 0; i < static_cast<int>(copy.size()); ++i) {
            const Patient& p = copy[i];
            std::cout << p.getPatientID() << "\t" << p.getFirstName() << " " << p.getLastName() << "\n";
        }
        std::cout << "\n";
        pauseForEnter();
    }
}

void sortPatientsByID(std::vector<Patient>& patients) {
    std::sort(patients.begin(), patients.end(), [](const Patient& a, const Patient& b){
        return a.getPatientID() < b.getPatientID();
    });
}

// Add equipment to storage unit chosen by runtime type detection
void addEquipmentToStorageUnits(std::vector<Equipment*>& equipment,
                                StorageUnit<Mobility>& mobilityUnit,
                                StorageUnit<Monitor>& monitorUnit) {
    if (equipment.empty()) {
        std::cout << "No equipment available.\n";
        pauseForEnter();
    } else {
        printEquipmentList(equipment);
        std::cout << "Enter equipment ID to add to a storage unit: ";
        int id = 0;
        std::cin >> id;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid entry.\n";
            pauseForEnter();
        } else {
            Equipment* found = nullptr;
            for (int i = 0; i < static_cast<int>(equipment.size()); ++i) {
                Equipment* e = equipment[i];
                if (e->getEquipmentID() == id) { found = e; break; }
            }
            if (!found) {
                std::cout << "The ID you entered is not in the system.  Please try again.\n";
                pauseForEnter();
            } else {
                // Check if this item already exists in either storage unit
                bool existsInMobility = false;
                for (int i = 0; i < mobilityUnit.getInventoryCount(); ++i) {
                    if (mobilityUnit.getInventoryItem(i).getEquipmentID() == id) { existsInMobility = true; break; }
                }
                bool existsInMonitor = false;
                for (int i = 0; i < monitorUnit.getInventoryCount(); ++i) {
                    if (monitorUnit.getInventoryItem(i).getEquipmentID() == id) { existsInMonitor = true; break; }
                }

                if (existsInMobility || existsInMonitor) {
                    std::cout << "This item has already been added to a storage unit. Operation cancelled.\n";
                    pauseForEnter();
                } else {
                    // Determine runtime type using typeid
                    std::string runtimeName = std::string(typeid(*found).name());

                    // Use substring match to be more robust across compilers
                    if (runtimeName.find("Mobility") != std::string::npos) {
                        Mobility* mptr = dynamic_cast<Mobility*>(found);
                        if (!mptr) {
                            std::cout << "Error: runtime type indicates Mobility but dynamic_cast failed.\n";
                            pauseForEnter();
                        } else {
                            try {
                                mobilityUnit.addEquipment(*mptr);
                                std::cout << "Equipment successfully added to Mobility storage unit.\n";
                                mobilityUnit.printInventory(std::cout);
                            } catch (const DuplicateIDException& ex) {
                                std::cout << ex.what() << " Operation cancelled.\n";
                            } catch (const std::exception& ex) {
                                std::cout << "Error adding equipment: " << ex.what() << "\n";
                            }
                            pauseForEnter();
                        }
                    }
                    else if (runtimeName.find("Monitor") != std::string::npos) {
                        Monitor* monptr = dynamic_cast<Monitor*>(found);
                        if (!monptr) {
                            std::cout << "Error: runtime type indicates Monitor but downcast failed.\n";
                            pauseForEnter();
                        } else {
                            try {
                                monitorUnit.addEquipment(*monptr);
                                std::cout << "Equipment successfully added to Monitor storage unit.\n";
                                monitorUnit.printInventory(std::cout);
                            } catch (const DuplicateIDException& ex) {
                                std::cout << ex.what() << " Operation cancelled.\n";
                            } catch (const std::exception& ex) {
                                std::cout << "Error adding equipment: " << ex.what() << "\n";
                            }
                            pauseForEnter();
                        }
                    }
                    else {
                        // If we get here, the runtime type wasn't one of the expected types
                        std::cout << "Selected equipment is not a Mobility or Monitor type. Operation cancelled.\n";
                        pauseForEnter();
                    }
                }
            }
        }
    }
}