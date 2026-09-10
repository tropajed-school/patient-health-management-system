#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Date.h"
#include "Equipment.h"
#include "Exceptions.h"

// StorageUnit template class definition
template<typename T>
class StorageUnit {
private:
    int unitID;
    std::string description;
    int lengthFeet;
    int widthFeet;
    int heightFeet;
    double costPerMonth;
    Date dateRented;
    bool humidityControlled;
    bool autoRenew;
    std::vector<T> inventory; // holds objects, not pointers

//
public:
	// constructor with default values
    StorageUnit(int id = 0,
                const std::string& desc = "",
                int lengthF = 0,
                int widthF = 0,
                int heightF = 0,
                double cost = 0.0,
                const Date& rented = Date(),
                bool humidity = false,
                bool autore = false)
        : unitID(id), description(desc),
          lengthFeet(lengthF), widthFeet(widthF), heightFeet(heightF),
          costPerMonth(cost), dateRented(rented),
          humidityControlled(humidity), autoRenew(autore) {}

    ~StorageUnit() = default;

    // accessors
    int getUnitID() const { return unitID; }
    std::string getDescription() const { return description; }
    int getLength() const { return lengthFeet; }
    int getWidth() const { return widthFeet; }
    int getHeight() const { return heightFeet; }
    double getCostPerMonth() const { return costPerMonth; }
    Date getDateRented() const { return dateRented; }
    bool isHumidityControlled() const { return humidityControlled; }
    bool doesAutoRenew() const { return autoRenew; }
    int getInventoryCount() const { return static_cast<int>(inventory.size()); }
    const T& getInventoryItem(int idx) const { return inventory.at(idx); }

    // mutators (throw NegativeNumberException on negative numeric inputs)
    void setUnitID(int id) { unitID = id; }
    void setDescription(const std::string& desc) { description = desc; }

    void setLength(int l) {
        if (l < 0) throw NegativeNumberException();
        lengthFeet = l;
    }
    void setWidth(int w) {
        if (w < 0) throw NegativeNumberException();
        widthFeet = w;
    }
    void setHeight(int h) {
        if (h < 0) throw NegativeNumberException();
        heightFeet = h;
    }
    void setCostPerMonth(double c) {
        if (c < 0.0) throw NegativeNumberException();
        costPerMonth = c;
    }
    void setDateRented(const Date& d) { dateRented = d; }
    void setHumidityControlled(bool v) { humidityControlled = v; }
    void setAutoRenew(bool v) { autoRenew = v; }

    // add equipment (throws DuplicateIDException if equipment ID already present)
    void addEquipment(const T& item) {
        // requires T to expose getEquipmentID()
        int id = item.getEquipmentID();
        for (const T& e : inventory) {
            if (e.getEquipmentID() == id) {
                throw DuplicateIDException();
            }
        }
        inventory.push_back(item);
    }

    // sort inventory using Equipment::operator<
    void sortInventory() {
        std::sort(inventory.begin(), inventory.end());
    }

    // print general unit information
    void printUnitInformation(std::ostream& out) const {
        out << "---------------------------------------------------------------------\n";
        out << "                         Storage Unit Information\n";
        out << "---------------------------------------------------------------------\n";
        out << "Unit ID:          " << unitID << "\n";
        out << "Description:      " << description << "\n";
        out << "Dimensions (LxWxH): " << lengthFeet << " x " << widthFeet << " x " << heightFeet << " feet\n";
        out << (humidityControlled ? "**Unit is humidity controlled.\n" : "**Unit is not humidity controlled.\n");
        out << "\nRental Contract\n";
        out << "Cost Per Month:   $" << std::fixed << std::setprecision(2) << costPerMonth << "\n";
        out << "Date Signed:      " << dateRented.toString() << "\n";
        out << (autoRenew ? "**Contract does auto-renew.\n" : "**Contract does not auto-renew.\n");
        out << "---------------------------------------------------------------------\n";
    }

    // print inventory (sort first)
    void printInventory(std::ostream& out) {
        sortInventory();
        out << "---------------------------------------------------------------------\n";
        out << "                         Storage Unit Inventory\n";
        out << "---------------------------------------------------------------------\n";
        if (inventory.empty()) {
            out << "No items in this storage unit.\n";
        } else {
            for (const T& item : inventory) {
                // T is derived from Equipment and should implement printDetails
                item.printDetails(out);
            }
        }
        out << "---------------------------------------------------------------------\n";
    }
};
