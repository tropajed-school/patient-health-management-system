#pragma once
#include "Equipment.h"
#include <string>

// Monitor class definition, derived from Equipment
class Monitor : public Equipment {
    std::string displayType; 
public:
    Monitor(int id = 0, const std::string& name = "", const Date& date = Date(),
            double cost = 0.0, int life = 0, double salvage = 0.0,
            const std::string& display = "");
    ~Monitor();

    std::string getDisplayType() const;
    void setDisplayType(const std::string& dt);

    void depreciate(std::ostream& out) const override;
    void printDetails(std::ostream& out) const override;

private:
    // recursive helper for straight-line
    void produceSchedule(std::ostream& out, const std::string& assetName,
                         int period, double bookValue, double salvage, int usefulLife,
                         double annualExpense, double totalDepreciation) const;
};
