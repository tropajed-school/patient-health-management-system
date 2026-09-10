#include "Patient.h"
#include <iostream>
#include <iomanip> 
#include <algorithm>    
// Default constructor
Patient::Patient()
    : patientID(0), firstName(""), lastName(""), streetAddress(""),
    age(0), height(0.0f), weight(0.0f), gender(' '), isAmbulatory(false), caseManager()
{}

// Multi-argument constructor
Patient::Patient(int id, const std::string& fn, const std::string& ln, const std::string& addr, 
                 int a, float h, float w, char g, bool amb)
    : patientID(id), firstName(fn), lastName(ln), streetAddress(addr),
      age(a), height(h), weight(w), gender(' '), isAmbulatory(amb), caseManager()
{
    // Normalize gender to uppercase 'M' or 'F' (store space if invalid)
    if (g == 'm' || g == 'M') gender = 'M';
    else if (g == 'f' || g == 'F') gender = 'F';
    else gender = ' ';
}

Patient::~Patient() {}

// Overloaded operators

// insertion operator 
std::ostream& operator<<(std::ostream& os, const Patient& p) {
    p.printPatientDetails(os);
    return os;
}

// less-than compares by firstName 
bool Patient::operator<(const Patient& other) const {
    return firstName < other.firstName;
}

// prefix ++ : increase age by 1 
Patient& Patient::operator++() {
    if (age < 120) {
        ++age;
    } else {
        std::cout << "Warning: Age is at maximum allowed value (120). Cannot increment further.\n";
    }
    return *this;
}

// postfix ++ : return previous value
Patient Patient::operator++(int) {
    Patient old = *this;
    ++(*this);
    return old;
}

// Accessors
int Patient::getPatientID() const { return patientID; }
std::string Patient::getFirstName() const { return firstName; }
std::string Patient::getLastName() const { return lastName; }
std::string Patient::getStreetAddress() const { return streetAddress; }
int Patient::getAge() const { return age; }
float Patient::getHeight() const { return height; }
float Patient::getWeight() const { return weight; }
char Patient::getGender() const { return gender; }
bool Patient::getIsAmbulatory() const { return isAmbulatory; }

 // Return a visit by index (copy). If invalid index, return default Visit().
Visit Patient::getVisit(int index) const {
    // single-return refactor: build a result and return it once at the end
    Visit result;
    if (index >= 0 && index < static_cast<int>(visits.size())) {
        result = visits[index];
    }
    return result;
}

int Patient::getNumVisits() const {
    return static_cast<int>(visits.size());
}

// Mutators with single return statement
bool Patient::setPatientID(int id) {
    bool idIsSet = false;
  
    if (id >= 1 && id <= 1000) {
        patientID = id;
        idIsSet = true;
    } else {
        std::cout << "Error: Patient ID must be between 1 and 1000." << std::endl;
    }
    return idIsSet;
}

// First Name validation: cannot be empty and cannot contain numbers
bool Patient::setFirstName(const std::string& fn) {
    bool nameIsSet = false;
    if (fn.empty()) {
        std::cout << "Error: First name cannot be empty." << std::endl;
    } else {
        // Check for numbers in the name
        bool hasNumbers = false;
        for (int i = 0; i < static_cast<int>(fn.size()); ++i) {
            char c = fn[i];
            if (c >= '0' && c <= '9') { hasNumbers = true; break; }
        }
        
        if (hasNumbers) {
            std::cout << "Error: First name cannot contain numbers." << std::endl;
        } else {
            firstName = fn;
            nameIsSet = true;
        }
    }
    return nameIsSet;
}

// Last Name validation: cannot be empty and cannot contain numbers
bool Patient::setLastName(const std::string& ln) {
    bool nameIsSet = false;
    if (ln.empty()) {
        std::cout << "Error: Last name cannot be empty." << std::endl;
    } else {
        // Check for numbers in the name
        bool hasNumbers = false;
        for (int i = 0; i < static_cast<int>(ln.size()); ++i) {
            char c = ln[i];
            if (c >= '0' && c <= '9') { hasNumbers = true; break; }
        }
        
        if (hasNumbers) {
            std::cout << "Error: Last name cannot contain numbers." << std::endl;
        } else {
            lastName = ln;
            nameIsSet = true;
        }
    }
    return nameIsSet;
}
// Street address validation: cannot be empty
bool Patient::setStreetAddress(const std::string& addr) {
    bool addressIsSet = false;
    if (addr.empty()) {
        std::cout << "Error: Street address cannot be empty." << std::endl;
    } else {
        streetAddress = addr;
        addressIsSet = true;
    }
    return addressIsSet;
}
// Age validation: must be between 1 and 120
bool Patient::setAge(int a) {
    bool ageIsSet = false;
    if (a >= 1 && a <= 120) {
        age = a;
        ageIsSet = true;
    } else {
        std::cout << "Error: Age must be between 1 and 120." << std::endl;
    }
    return ageIsSet;
}
// Height validation: must be between 12 and 96 inches
bool Patient::setHeight(float h) {
    bool heightIsSet = false;
    if (h >= 12.0f && h <= 96.0f) { 
        height = h;
        heightIsSet = true;
    } else {
        std::cout << "Error: Height must be between 12 and 96 inches." << std::endl;
    }
    return heightIsSet;
}
// Weight validation: must be between 1 and 1000 pounds
bool Patient::setWeight(float w) {
    bool weightIsSet = false;
    if (w >= 1.0f && w <= 1000.0f) { 
        weight = w;
        weightIsSet = true;
    } else {
        std::cout << "Error: Weight must be between 1 and 1000 pounds." << std::endl;
    }
    return weightIsSet;
}
//Gender validation: must be M or F (case-insensitive)
bool Patient::setGender(char g) {
    bool genderIsSet = false;
    if (g == 'M' || g == 'F' || g == 'm' || g == 'f') {
        gender = (g == 'm') ? 'M' : (g == 'f') ? 'F' : g; // Convert to uppercase
        genderIsSet = true;
    } else {
        std::cout << "Error: Gender must be M or F." << std::endl;
    }
    return genderIsSet;
}

void Patient::setIsAmbulatory(bool amb) { isAmbulatory = amb; }
// Case Manager accessors/mutators
void Patient::printPatientDetails(std::ostream& out) const {
    out << "---------------------------------------------------------------------\n";
    out << std::left << std::setw(17) << "Patient ID:" << getPatientID() << "\n";
    out << std::left << std::setw(17) << "Patient Name:" << getFirstName() << " " << getLastName() << "\n";
    out << std::left << std::setw(17) << "Address:" << getStreetAddress() << "\n";
    out << std::left << std::setw(17) << "Age:" << getAge() << "\n";
    out << std::left << std::setw(17) << "Height:" << getHeight() << " inches\n";
    out << std::left << std::setw(17) << "Weight:" << getWeight() << " pounds\n";
    out << std::left << std::setw(17) << "Gender:" << (getGender() == 'M' ? "Male" : "Female") << "\n";
    out << "    *** Patient is " << (getIsAmbulatory() ? "" : "not ") << "ambulatory ***\n\n";

    // Case manager info
    out << std::left << std::setw(17) << "Case Manager:";
    if (caseManager.getEmployeeID() != 0) {
        out << caseManager.getFirstName() << " " << caseManager.getLastName() << "\n\n";
    } else {
        out << "None assigned\n\n";
    }

    // Print BPM readings and statistics
    if (!bpmReadings.empty()) {
        out << "Heart rate (bpm) readings:\n";
        std::vector<int> sortedReadings = bpmReadings;
        std::sort(sortedReadings.begin(), sortedReadings.end());

        for (int i = 0; i < static_cast<int>(sortedReadings.size()); i++) {
            out << sortedReadings[i];
            if (i < static_cast<int>(sortedReadings.size()) - 1) out << "  ";
        }
        out << "\n\n";
        out << std::setw(30) << "Patient statistics\n\n";
        out << std::left << std::setw(30) << "Number of bpm readings:" << getNumReadings() << "\n";
        out << std::left << std::setw(30) << "Average heart rate (bpm):" << std::fixed << std::setprecision(1) 
            << calculateAverageBpm() << "\n";
        out << std::left << std::setw(30) << "Minimum heart rate (bpm):" << findMinimumBpm() << "\n";
        out << std::left << std::setw(30) << "Maximum heart rate (bpm):" << findMaximumBpm() << "\n";
    }

    // Print visit information
    if (!visits.empty()) {
        out << "\nVisit Information:\n";
        for (int i = 0; i < static_cast<int>(visits.size()); i++) {
            visits[i].printVisit(out);
        }
    }

    out << "\n";
}

// BPM functions
void Patient::addBpmReading(int bpm) { bpmReadings.push_back(bpm); }
int Patient::getBpmReading(int index) const {
    int reading = -1;
    if (index >= 0 && index < static_cast<int>(bpmReadings.size())) reading = bpmReadings[index];
    return reading;
}
int Patient::getNumReadings() const { return static_cast<int>(bpmReadings.size()); }
void Patient::sortBpmReadings() {
    int n = static_cast<int>(bpmReadings.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (bpmReadings[j] < bpmReadings[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            int tmp = bpmReadings[i];
            bpmReadings[i] = bpmReadings[minIndex];
            bpmReadings[minIndex] = tmp;
        }
    }
}
// Find minimum bpm reading; returns -1 if no readings
int Patient::findMinimumBpm() const {
    int minimum = -1;
    if (!bpmReadings.empty()) {
        minimum = bpmReadings[0];
        for (int i = 1; i < static_cast<int>(bpmReadings.size()); i++)
            if (bpmReadings[i] < minimum) minimum = bpmReadings[i];
    }
    return minimum;
}
// Find maximum bpm reading; returns -1 if no readings
int Patient::findMaximumBpm() const {
    int maximum = -1;
    if (!bpmReadings.empty()) {
        maximum = bpmReadings[0];
        for (int i = 1; i < static_cast<int>(bpmReadings.size()); i++)
            if (bpmReadings[i] > maximum) maximum = bpmReadings[i];
    }
    return maximum;
}
// Calculate average bpm; returns 0.0 if no readings
double Patient::calculateAverageBpm() const {
    double average = 0.0;
    if (!bpmReadings.empty()) {
        int sum = 0;
        for (int i = 0; i < static_cast<int>(bpmReadings.size()); ++i) sum += bpmReadings[i];
        average = static_cast<double>(sum) / static_cast<double>(bpmReadings.size());
    }
    return average;
}

 // Visit functions
void Patient::addVisit(const Visit& visit) { visits.push_back(visit); }

 // Case manager accessors/mutators
void Patient::setCaseManager(const CaseManager& cm) { caseManager = cm; }
CaseManager Patient::getCaseManager() const { return caseManager; }