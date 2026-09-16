Patient Management System

**Class Course**  
**Date:** September 2026  

---

## Project Overview
The C++ system manages patient health records, home-health visits, service assignments, medical personnel, and durable medical equipment (DME). It incorporates object composition, dynamic memory management, polymorphism, operator overloading, templates, and exception handling.

---

## Key Features

* **Patient & Clinical Records Management**
  * Tracks demographics, ambulatory status, heart rate (bpm) readings, case managers, and visit histories.
  * Computes heart rate statistics (minimum, maximum, average) dynamically.
  * Generates unique Patient IDs from 1 to 1000 using `std::uniform_int_distribution`.
  * Overloads prefix/postfix `++` operators to increment patient age.

* **Visits & Service Tracking**
  * Assigns nurses or aides and standardized services to patient visits.
  * Standardizes visit dates (`mm/dd/yyyy`) via character array tokenization into `Date` objects.
  * Enforces unique Visit IDs per patient with custom exception handling.

* **Durable Medical Equipment (DME) & Depreciation**
  * Polymorphically processes DME items through base class `Equipment` pointers.
  * **Mobility Aids:** Useful life > 5 years; calculated recursively using the Double-Declining Balance (DDB) method.
  * **Monitoring Devices:** Useful life <= 5 years; calculated recursively using the Straight-Line method.
  * Formats tabular depreciation schedules for screen or file export.

* **Storage Unit Management**
  * Implemented as a **Class Template** (`StorageUnit<T>`) to hold inventories of either `Mobility` or `Monitor` devices.
  * Utilizes `typeid` and `dynamic_cast` downcasting to assign dynamically allocated equipment objects to their correct storage units.
  * Sorts inventory in each unit by equipment ID.

---

## Technical Architecture

| Category | Implementation Details |
| :--- | :--- |
| **Object-Oriented Design** | Abstract Base Class `Equipment` with pure virtual `depreciate()`; derived classes `Mobility` and `Monitor`. Object composition across `Patient`, `Visit`, `Service`, and `Date`. |
| **Templates** | Generic `StorageUnit<T>` class template and generic client helper functions. |
| **Operator Overloading** | Overloaded stream insertion `<<` for formatted patient display; `++` for patient age; `<` for sorting patients by first name and equipment by ID. |
| **Dynamic Memory** | Deep-copy constructors and overloaded assignment operators (`=`) in employee classes; dynamic array pointers. |
| **Exception Handling** | Custom exceptions inheriting from `std::runtime_error`: `NegativeNumberException`, `DuplicateIDException`, and `InvalidDateException`. |
| **STL & Algorithms** | `std::vector`, lambda expressions for sorting pointers to base objects, and `<algorithm>` sorting routines. |

---

## Menu Structure

The user interface utilizes a categorized menu system:

* **1. Patients**
  * Add new patient manually
  * Read patient data from file
  * Delete existing patient
  * Create visit for patient
  * Assign case manager to patient
  * Print patient report
  * Save patient report
  * Increase patient age
* **2. Employees**
  * Print employee report
* **3. Equipment**
  * Print equipment list
  * Produce depreciation schedule
  * Update storage unit information
  * Add equipment to storage unit
  * View storage unit information
* **4. Exit**
