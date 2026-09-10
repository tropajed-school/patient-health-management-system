#pragma once	
#include <string>
#include <vector>
#include <ostream>
#include "Visit.h"
#include "CaseManager.h"
// Patient class definition
class Patient {
private:
	int patientID;
	std::string firstName;
	std::string lastName;
	std::string streetAddress;
	int age;
	float height;
	float weight;
	char gender;
	bool isAmbulatory;
	std::vector<int> bpmReadings;
	std::vector<Visit> visits;  //Add vector to store visits
    CaseManager caseManager;    

public:
	Patient(); // Default constructor
	Patient(int id, const std::string& fn, const std::string& ln, const std::string& addr, // Multi-argument constructor
	        int a, float h, float w, char g, bool amb);
	~Patient(); // Destructor

	// Overloaded operators
	bool operator<(const Patient& other) const; // compare by firstName for std::sort
	Patient& operator++();   // prefix
	Patient operator++(int); // postfix

	// Accessors
	int getPatientID() const;
	std::string getFirstName() const;
	std::string getLastName() const;
	std::string getStreetAddress() const;
	int getAge() const;
	float getHeight() const;
	float getWeight() const;
	char getGender() const;
	bool getIsAmbulatory() const;
	// Mutators
	bool setPatientID(int id);
	bool setFirstName(const std::string& fn);
	bool setLastName(const std::string& ln);
	bool setStreetAddress(const std::string& addr);
	bool setAge(int a);
	bool setHeight(float h);
	bool setWeight(float w);
	bool setGender(char g);
	void setIsAmbulatory(bool amb);
	//print function
	void printPatientDetails(std::ostream& os) const;

	// BPM functions:
	void addBpmReading(int bpm);
	int getBpmReading(int index) const;
	int getNumReadings() const;
	void sortBpmReadings();
	int findMinimumBpm() const;
	int findMaximumBpm() const;
	double calculateAverageBpm() const;
	
	// Visit functions
	void addVisit(const Visit& visit);
	// New: return a single Visit by index and number of visits
	Visit getVisit(int index) const;
	int getNumVisits() const;

    // Case Manager accessors/mutators
    void setCaseManager(const CaseManager& cm);
    CaseManager getCaseManager() const;
};
// Overloaded insertion operator for Patient
std::ostream& operator<<(std::ostream& os, const Patient& p);

