#pragma once

#include <string>
#include <iostream>
#include "Date.h"

// Base Equipment class with pure virtual function for depreciation schedule
class Equipment {
protected:
    int equipmentID;
    std::string equipmentName;
    Date purchaseDate;
    double originalCost;
    int usefulLife;
    double salvageValue;
//
public:
    Equipment(int id = 0, const std::string& name = "", const Date& date = Date(),
		double cost = 0.0, int life = 0, double salvage = 0.0); // six-argument constructor with default parameters
    virtual ~Equipment();

    // accessors
    int getEquipmentID() const;
    std::string getEquipmentName() const;
    Date getPurchaseDate() const;
    double getOriginalCost() const;
    int getUsefulLife() const;
    double getSalvageValue() const;

    // mutators
    void setEquipmentID(int id);
    void setEquipmentName(const std::string& name);
    void setPurchaseDate(const Date& date);
    void setOriginalCost(double cost);
    void setUsefulLife(int life);
    void setSalvageValue(double salvage);

    // polymorphic interface
    virtual void depreciate(std::ostream& out) const = 0;
    virtual void printDetails(std::ostream& out) const;

    // compare by equipmentID
    bool operator<(const Equipment& other) const;
};
