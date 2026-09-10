#pragma once

#include <string>

// Simple date type: no operator overloads, only getters/setters.
class Date 
{
public:
    Date(int =0, int =0, int = 0);
    ~Date();
    void setDate(int, int, int);

    // simple accessors
    int getMonth() const;
    int getDay() const;
    int getYear() const;

    // Return formatted "mm/dd/yyyy"
    std::string toString() const;

private:
    int month;
    int day;
    int year;
};
