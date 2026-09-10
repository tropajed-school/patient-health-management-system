#pragma once
#include "Equipment.h"
#include <string>

// Mobility class definition, derived from Equipment
class Mobility : public Equipment {
    std::string operationMode; 
public:
    Mobility(int id = 0, const std::string& name = "", const Date& date = Date(),
             double cost = 0.0, int life = 0, double salvage = 0.0,
             const std::string& mode = "");
    ~Mobility();
	
    std::string getOperationMode() const;
    void setOperationMode(const std::string& mode);
    
    void depreciate(std::ostream& out) const override;
    void printDetails(std::ostream& out) const override;

private:
    // recursive helper for double-declining balance
    void produceSchedule(std::ostream& out, const std::string& assetName,
                         int period, double bookValue, double salvage, int usefulLife,
                         double totalDepreciation) const;
};
