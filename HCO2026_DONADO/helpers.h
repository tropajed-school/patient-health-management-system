#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

//Template is used to load employee data from files into vectors.
template <typename T>
void loadEmployeeData(std::vector<T>& vec, const std::string& fileName)
{
    std::ifstream in(fileName);
    if (!in.is_open()) {
        std::cout << "Warning: Could not open " << fileName << ". No data loaded.\n";
    } else {
        int id;
        while (in >> id) {
            in.ignore(); // skip newline after id
            std::string first, last, extra;
            std::getline(in, first);
            std::getline(in, last);
            float pay = 0.0f;
            if (!(in >> pay)) {
              
                in.clear();
                in.ignore(1000, '\n');
                continue;
            }
            in.ignore(); // skip newline before extra field
            std::getline(in, extra); 

            vec.emplace_back(id, first, last, pay, extra);
        }

        in.close();
        std::cout << "Loaded data from " << fileName << "\n";
    }
}

// Show a simple selection list of employees 
template <typename T>
void showEmployees(const std::vector<T>& vec, const std::string& title)
{
    std::cout << "\n" << title << "\n\n";
    std::cout << std::left << std::setw(30) << "Name" << "ID\n\n";
    for (int i = 0; i < static_cast<int>(vec.size()); ++i) {
        const T& e = vec[i];
        std::cout << std::left << std::setw(30) << (e.getFirstName() + " " + e.getLastName())
            << e.getEmployeeID() << "\n";
    }
    std::cout << "\n";
}

// Find employee by ID (returns index or -1) 
template <typename T>
int findEmployee(int id, const std::vector<T>& vec)
{
    int result = -1;
    for (int i = 0; i < static_cast<int>(vec.size()); ++i) {
        if (vec[i].getEmployeeID() == id) {
            result = i;
            break;
        }
    }
    return result;
}

// Use std::sort to sort employees by last name 
template <typename T>
void sortEmployeesByLastName(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end(), [](const T& a, const T& b) {
        return a.getLastName() < b.getLastName();
    });
}


// Helper to build a vector of employee objects from an array of pointers (ex. from file loading)
template <typename T>
std::vector<T> buildEmployeeVector(T** arr, int count) {
    std::vector<T> result;
    if (!arr || count <= 0) return result;
    result.reserve(static_cast<size_t>(count));
    for (int i = 0; i < count; ++i) {
        if (arr[i]) result.push_back(*arr[i]); // copy object
    }
    return result;
}