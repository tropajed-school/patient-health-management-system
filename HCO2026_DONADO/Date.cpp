#include "Date.h"
#include <sstream>
#include <iomanip>
// Constructor with parameters
Date::Date(int m, int d, int yyyy)
{
    setDate(m, d, yyyy);
}
// Destructor 
Date::~Date() {}
// Mutator: set all date components at once
void Date::setDate(int m, int d, int yyyy)
{
    month = m;
    day = d;
    year = yyyy;
}
// Accessors
int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }
int Date::getYear() const { return year; }
// Return formatted date string "mm/dd/yyyy" with the first zeros for month/day
std::string Date::toString() const
{
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << month << '/'
        << std::setw(2) << std::setfill('0') << day << '/'
        << year;
    return oss.str();
}


