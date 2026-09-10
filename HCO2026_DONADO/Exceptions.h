#pragma once

#include <stdexcept>
#include <string>

//Exception for negative numbers (e.g. age, height, weight)
class NegativeNumberException : public std::runtime_error {
public:
    NegativeNumberException() : std::runtime_error("Only positive numbers are allowed.") {}
};
//Exception for duplicate IDs (e.g. patient ID)
class DuplicateIDException : public std::runtime_error {
public:
    DuplicateIDException() : std::runtime_error("This ID already exists.") {}
};
//Exception for invalid date format (e.g. MM/DD/YYYY)
class InvalidDateException : public std::runtime_error {
public:
    InvalidDateException() : std::runtime_error("Invalid date format.") {}
};
