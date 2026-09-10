#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>

// Constants
const int SCREEN_WIDTH{ 80 };

// Function prototypes
int calculateOffset(const std::string& s);
void clearCin();
void wait();
template<typename T>
void promptForNumericInput(const std::string& prompt, T& value);
void promptForStringInput(const std::string& prompt, std::string& value);

void printMenu();
int getMenuSelection();

void printReport(std::ostream& out,
                 const std::string& assetName,
                 double originalCost,
                 double salvageValue,
                 int usefulLife);

void depreciate(std::ostream& out,
                double beginValue,
                double salvageValue,
                int usefulLife,
                int period,
                double dblRate,
                double& totalDep);

// Entry point
int main()
{
    enum MenuOptions { INPUTS = 1, SCREEN_PRINT, FILE_PRINT, QUIT };

    std::string assetName;
    double originalCost{ 0.0 };
    double salvageValue{ 0.0 };
    int usefulLife{ 0 };

    bool inputsEntered = false;
    int choice = 0;

    do
    {
        printMenu();
        choice = getMenuSelection();

        switch (choice)
        {
        case INPUTS:
        {
            // Gather inputs with validation
            promptForStringInput("\nEnter the name of the asset: ", assetName);

            do
            {
                promptForNumericInput("Enter the asset's original cost: ", originalCost);
                if (originalCost <= 0.0)
                {
                    std::cout << "The value you entered was not positive. Please try again.\n";
                }
            } while (originalCost <= 0.0);

            do
            {
                promptForNumericInput("Enter the asset's salvage value: ", salvageValue);
                if (salvageValue < 0.0)
                {
                    std::cout << "The value you entered was negative. Please try again.\n";
                }
                else if (salvageValue >= originalCost)
                {
                    std::cout << "Salvage value must be less than the original cost. Please try again.\n";
                }
            } while (salvageValue < 0.0 || salvageValue >= originalCost);

            do
            {
                promptForNumericInput("Enter the asset's useful life (in years): ", usefulLife);
                if (usefulLife <= 0)
                {
                    std::cout << "The value you entered was not a positive integer. Please try again.\n";
                }
            } while (usefulLife <= 0);

            inputsEntered = true;
            std::cout << "\nInputs recorded.\n\n";
            std::cout << "Press the Enter key to continue ...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case SCREEN_PRINT:
        {
            if (!inputsEntered)
            {
                std::cout << "\nNo inputs entered. Please choose option 1 first.\n";
                std::cout << "\nPress the Enter key to continue ...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
            else
            {
                system("cls");
                printReport(std::cout, assetName, originalCost, salvageValue, usefulLife);
                wait();
            }
            break;
        }
        case FILE_PRINT:
        {
            if (!inputsEntered)
            {
                std::cout << "\nNo inputs entered. Please choose option 1 first.\n";
                std::cout << "\nPress the Enter key to continue ...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
            else
            {
                clearCin();
                std::string filename;
                std::cout << "\nEnter filename (may include spaces): ";
                std::getline(std::cin, filename);

                std::ofstream outFile(filename);
                if (!outFile.is_open())
                {
                    std::cout << "Error: Could not open file \"" << filename << "\" for writing.\n";
                    std::cout << "\nPress the Enter key to continue ...";
                    std::cin.get();
                }
                else
                {
                    printReport(outFile, assetName, originalCost, salvageValue, usefulLife);
                    outFile.close();
                    std::cout << "\nReport written to file \"" << filename << "\" successfully.\n";
                    std::cout << "\nPress the Enter key to continue ...";
                    std::cin.get();
                }
            }
            break;
        }
        case QUIT:
        {
            std::cout << "\nThank you for using the Health Care Options Depreciation Calculator.\n";
            std::cout << "Goodbye.\n";
            break;
        }
        default:
            // Shouldn't happen because getMenuSelection validates
            break;
        }

    } while (choice != QUIT);

    return 0;
}

// --------------------- Implementations ---------------------

int calculateOffset(const std::string& s)
{
    return SCREEN_WIDTH / 2 + static_cast<int>(s.size()) / 2;
}

void clearCin()
{
    if (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        // If cin is in fail state but buffer empty, still clear flags
        if (std::cin.fail())
            std::cin.clear();
    }
}

void wait()
{
    clearCin();
    char ch;
    std::cout << std::endl << "Press the Enter key to continue ... ";
    std::cin.get(ch);
}

template<typename T>
void promptForNumericInput(const std::string& prompt, T& value)
{
    do
    {
        clearCin();
        std::cout << prompt;
        std::cin >> value;
        if (!std::cin)
        {
            std::cout << "The value you entered was not a number. Please try again.\n";
        }
        else if (value < 0)
        {
            std::cout << "The value you entered was negative. Please try again.\n";
        }
    } while (!std::cin || value < 0);
}

void promptForStringInput(const std::string& prompt, std::string& value)
{
    clearCin();
    std::cout << prompt;
    std::getline(std::cin, value);
}

// Menu helpers
void printMenu()
{
    std::cout << "****************************************************************\n";
    std::cout << std::setw(calculateOffset("Health Care Options, Inc.") ) << std::right << "Health Care Options, Inc." << "\n";
    std::cout << std::setw(calculateOffset("Depreciation Calculator") ) << std::right << "Depreciation Calculator" << "\n";
    std::cout << "****************************************************************\n\n";

    std::cout << "1. Enter inputs\n";
    std::cout << "2. Print depreciation schedule to screen\n";
    std::cout << "3. Print depreciation schedule to file\n";
    std::cout << "4. Exit\n\n";
}

int getMenuSelection()
{
    int sel = 0;
    bool valid = false;
    do
    {
        std::cout << "Enter selection: ";
        std::cin >> sel;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "The value you entered was not a number. Please try again.\n";
        }
        else if (sel < 1 || sel > 4)
        {
            std::cout << "Please enter a number between 1 and 4.\n";
        }
        else
        {
            valid = true;
        }
    } while (!valid);

    return sel;
}

// Print report (to either cout or an ofstream) using the same function
void printReport(std::ostream& out,
                 const std::string& assetName,
                 double originalCost,
                 double salvageValue,
                 int usefulLife)
{
    const std::string headerTitle1{ "Health Care Options, Inc." };
    const std::string headerTitle2{ "Depreciation Schedule" };

    out << "****************************************************************\n";
    out << std::setw(calculateOffset(headerTitle1)) << std::right << headerTitle1 << "\n";
    out << std::setw(calculateOffset(headerTitle2)) << std::right << headerTitle2 << "\n";
    out << "****************************************************************\n\n";

    out << std::left << std::setw(20) << "Asset name:" << " " << assetName << "\n";
    out << std::left << std::setw(20) << "Original cost:" << " $" << std::fixed << std::setprecision(2) << originalCost << "\n";
    out << std::left << std::setw(20) << "Salvage value:" << " $" << std::fixed << std::setprecision(2) << salvageValue << "\n";
    out << std::left << std::setw(20) << "Useful life (in years):" << " " << usefulLife << "\n\n";

    // Table header
    out << std::left << std::setw(8) << "Period"
        << std::setw(26) << "Depreciation Expense ($)"
        << std::setw(20) << "Book Value ($)" << "\n";

    // Show starting book value on first line as in sample (we will print ending book value per row)
    double dblRate = 2.0 * (1.0 / static_cast<double>(usefulLife)); // double-declining rate
    double totalDep = 0.0;

    // Start recursion: period 1
    depreciate(out, originalCost, salvageValue, usefulLife, 1, dblRate, totalDep);

    out << "\n";
    out << std::left << std::setw(20) << "Total Depreciation:" << " " << std::fixed << std::setprecision(2) << totalDep << "\n\n";

    out << "****************************************************************\n";
    out << "                         End of Report\n";
    out << "****************************************************************\n";
}

// Recursive depreciate function
// Prints each period line and accumulates totalDep.
// This implementation follows the "one return per function" guideline:
// depreciate contains no early returns; control flows to the single exit point.
void depreciate(std::ostream& out,
                double beginValue,
                double salvageValue,
                int usefulLife,
                int period,
                double dblRate,
                double& totalDep)
{
    bool stop = false;

    if (period > usefulLife)
    {
        stop = true;
    }
    else if (beginValue <= salvageValue)
    {
        stop = true;
    }

    if (!stop)
    {
        double expense = beginValue * dblRate;
        double endingBook = beginValue - expense;

        if (endingBook < salvageValue || period == usefulLife)
        {
            expense = beginValue - salvageValue;
            endingBook = salvageValue;
        }

        out << std::right << std::setw(6) << period << "  "
            << std::setw(18) << std::fixed << std::setprecision(2) << expense << "  "
            << std::setw(12) << std::fixed << std::setprecision(2) << endingBook << "\n";

        totalDep += expense;

        depreciate(out, endingBook, salvageValue, usefulLife, period + 1, dblRate, totalDep);
    }
}