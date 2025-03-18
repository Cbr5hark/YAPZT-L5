#define _CRT_SECURE_NO_WARNINGS
// Prison Management System - Refactored Version

#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <vector>
#include <iomanip>

using namespace std;

// Constants
const string USERNAME = "admin";
const string PASSWORD = "pass";
const int MAX_PRISONERS = 20;
const int BASE_CELL_NUMBER = 1001;

// Forward declarations
void displayDateTime();
void displayLoadingAnimation(const string& message, int numDots = 15);
void displayHeader(const string& title);

// Prisoner class to represent a single prisoner
class Prisoner {
private:
    string firstName;
    string secondName;
    string gender;
    int cellNo;
    int age;
    double height;
    string eyeColor;
    string crime;
    int punishmentMonths;
    bool isOccupied;

public:
    // Constructor
    Prisoner(int cellId = 0) :
        firstName(""), secondName(""), gender(""), cellNo(BASE_CELL_NUMBER + cellId),
        age(0), height(0.0), eyeColor(""), crime(""), punishmentMonths(0),
        isOccupied(false) {}

    // Getters
    string getFirstName() const { return firstName; }
    string getSecondName() const { return secondName; }
    string getFullName() const { return firstName + " " + secondName; }
    string getGender() const { return gender; }
    int getCellNo() const { return cellNo; }
    int getAge() const { return age; }
    double getHeight() const { return height; }
    string getEyeColor() const { return eyeColor; }
    string getCrime() const { return crime; }
    int getPunishmentMonths() const { return punishmentMonths; }
    bool isCell() const { return isOccupied; }

    // Setters
    void setFirstName(const string& name) { firstName = name; }
    void setSecondName(const string& name) { secondName = name; }
    void setGender(const string& g) { gender = g; }
    void setAge(int a) { age = a; }
    void setHeight(double h) { height = h; }
    void setEyeColor(const string& color) { eyeColor = color; }
    void setCrime(const string& c) { crime = c; }
    void setPunishmentMonths(int months) { punishmentMonths = months; }
    void setOccupied(bool status) { isOccupied = status; }

    // Methods
    void decreasePunishment() { punishmentMonths--; }
    bool isReadyForRelease() const { return punishmentMonths <= 0; }
    void clearData() {
        firstName = "";
        secondName = "";
        gender = "";
        age = 0;
        height = 0.0;
        eyeColor = "";
        crime = "";
        punishmentMonths = 0;
        isOccupied = false;
    }
};

// Prison Management System class
class PrisonManagementSystem {
private:
    vector<Prisoner> prisoners;
    bool isLoggedIn;

    // Helper methods
    void displayPrisonerTable(const string& title) const;
    void displayMenu() const;
    bool validateLogin(const string& username, const string& password) const;
    int getCellIndexFromNumber(int cellNumber) const;

public:
    // Constructor
    PrisonManagementSystem() : isLoggedIn(false) {
        // Initialize prisoners with cell numbers
        prisoners.resize(MAX_PRISONERS);
        for (int i = 0; i < MAX_PRISONERS; i++) {
            prisoners[i] = Prisoner(i);
        }
    }

    // Main functions
    void login();
    void logout();
    void mainMenu();
    void addPrisoner();
    void showPrisonerDetails();
    void manageAttendance();
    void releasePrisoner();
    void searchPrisoner();
    void exportPrisonData();
    void exit();
};

// Utility functions
void displayDateTime() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    cout << "Time: " << setfill('0') << setw(2) << localTime->tm_hour << ":"
        << setfill('0') << setw(2) << localTime->tm_min << ":"
        << setfill('0') << setw(2) << localTime->tm_sec << endl;

    cout << "\n\n\n";
    cout << "\t\t\t\t\t   Date: " << localTime->tm_mday << "/"
        << localTime->tm_mon + 1 << "/" << localTime->tm_year + 1900 << "\n";
}

void displayLoadingAnimation(const string& message, int numDots) {
    cout << "\n\t\t\t\t" << message;
    for (int i = 0; i < numDots; i++) {
        Sleep(100); // Use Sleep instead of empty loops
        cout << ".";
    }
    cout << endl;
}

void displayHeader(const string& title) {
    system("cls");
    displayDateTime();

    cout << "\t\t\t\t    -----------------------------------------------------\n";
    cout << "\t\t\t\t    |                                                   |\n";
    cout << "\t\t\t\t    |       " << left << setw(40) << title << "|\n";
    cout << "\t\t\t\t    |                                                   |\n";
    cout << "\t\t\t\t    -----------------------------------------------------\n\n";
}

// Implementation of PrisonManagementSystem methods
void PrisonManagementSystem::login() {
    string username, password;
    char ch;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        system("cls");
        displayDateTime();

        cout << "\n\n\n\n\n\n";
        cout << "\t\t\t\t    ----------------------------------------\n";
        cout << "\t\t\t\t    |   PRISON STATION MANAGEMENT SYSTEM   |\n";
        cout << "\t\t\t\t    ----------------------------------------\n\n\n";

        cout << "\t\t\t\t\tUsername: ";
        cin >> username;

        cout << "\t\t\t\t\t\nPassword: ";

        // Secure password input
        password = "";
        while (true) {
            ch = _getch();
            if (ch == 13) // Enter key
                break;
            else if (ch == 8) { // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) { // Printable characters
                password += ch;
                cout << "*";
            }
        }

        if (validateLogin(username, password)) {
            cout << "\n\n\t\t\t\tYou are accessed to the system!\n\n";
            cout << "\t\t\t\t";
            system("pause");
            isLoggedIn = true;
            mainMenu();
            return;
        }
        else {
            attempts++;
            system("cls");
            cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            cout << "\t\t\t\t\tNo. of attempts remain: " << MAX_ATTEMPTS - attempts;
            cout << "\n\n\t\t\t\t";
            system("pause");
        }
    }

    cout << "\n\t\t\t\tNo permission to enter the system!\n\n";
    cout << "\t\t\t\t";
    system("pause");
}

bool PrisonManagementSystem::validateLogin(const string& username, const string& password) const {
    return (username == USERNAME && password == PASSWORD);
}

void PrisonManagementSystem::logout() {
    system("cls");
    displayLoadingAnimation("Logging out", 15);
    isLoggedIn = false;
    login();
}

void PrisonManagementSystem::displayMenu() const {
    cout << "\n\n\n";
    cout << "\t\t\t\t---------------------------------------------------------------------------\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     1)   New prisoner entry                             |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     2)   Prisoner details                               |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     3)   Attendance prisoner                            |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     4)   Release prisoner                               |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     5)   Search prisoner                                |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     6)   Prison File                                    |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     7)   Logout                                         |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t|                     8)   Exit                                           |\n";
    cout << "\t\t\t\t|                                                                         |\n";
    cout << "\t\t\t\t---------------------------------------------------------------------------\n";
}

void PrisonManagementSystem::mainMenu() {
    int choice;

    do {
        system("cls");
        displayDateTime();
        displayMenu();

        cout << "\n\t\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addPrisoner();
            break;
        case 2:
            showPrisonerDetails();
            break;
        case 3:
            manageAttendance();
            break;
        case 4:
            releasePrisoner();
            break;
        case 5:
            searchPrisoner();
            break;
        case 6:
            exportPrisonData();
            break;
        case 7:
            logout();
            return;
        case 8:
            exit();
            return;
        default:
            cout << "\n\t\t\t\tInvalid choice\n";
            cout << "\t\t\t\tPress any key to return...";
            _getch();
        }
    } while (true);
}

void PrisonManagementSystem::displayPrisonerTable(const string& title) const {
    displayHeader(title);

    cout << "\n===========================================================================================================\n";
    cout << "NAME\t\t\tCell Block\tAge\tGender\tHeight\tEye Colour\tCrime\tPunishment\n";
    cout << "===========================================================================================================\n";

    int count = 0;
    for (const auto& prisoner : prisoners) {
        if (prisoner.isCell()) {
            cout << prisoner.getFullName() << "\t\t"
                << prisoner.getCellNo() << "\t\t"
                << prisoner.getAge() << "\t"
                << prisoner.getGender() << "\t"
                << prisoner.getHeight() << "\t"
                << prisoner.getEyeColor() << "\t\t"
                << prisoner.getCrime() << "\t\t"
                << prisoner.getPunishmentMonths() << endl;
            cout << "\n";
            count++;
        }
    }

    if (count == 0) {
        cout << "\n\t\t\t\tNo prisoners present\n";
    }
}

int PrisonManagementSystem::getCellIndexFromNumber(int cellNumber) const {
    return cellNumber - BASE_CELL_NUMBER;
}

void PrisonManagementSystem::addPrisoner() {
    displayHeader("Enter basic information of prisoner");

    // Find an empty cell
    int index = -1;
    for (int i = 0; i < MAX_PRISONERS; i++) {
        if (!prisoners[i].isCell()) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "\n\t\t\t\tNo empty cells available!\n";
        cout << "\t\t\t\tPress any key to return...";
        _getch();
        return;
    }

    string firstName, secondName, gender, eyeColor, crime;
    int age, punishmentMonths;
    double height;

    cout << "\t\t\t\t\tEnter first name: ";
    cin >> firstName;
    cout << "\t\t\t\t\t\nEnter second name: ";
    cin >> secondName;
    cout << "\t\t\t\t\t\nEnter gender: ";
    cin >> gender;
    cout << "\t\t\t\t\t\nEnter age: ";
    cin >> age;
    cout << "\t\t\t\t\t\nEnter height: ";
    cin >> height;
    cout << "\t\t\t\t\t\nEnter eye color: ";
    cin >> eyeColor;
    cout << "\t\t\t\t\t\nEnter crime: ";
    cin >> crime;
    cout << "\t\t\t\t\t\nEnter punishment span in months: ";
    cin >> punishmentMonths;

    prisoners[index].setFirstName(firstName);
    prisoners[index].setSecondName(secondName);
    prisoners[index].setGender(gender);
    prisoners[index].setAge(age);
    prisoners[index].setHeight(height);
    prisoners[index].setEyeColor(eyeColor);
    prisoners[index].setCrime(crime);
    prisoners[index].setPunishmentMonths(punishmentMonths);
    prisoners[index].setOccupied(true);

    cout << "\n\n\t\t\t\t\tYour Cell No is " << prisoners[index].getCellNo() << endl;
    cout << "\n\t\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::showPrisonerDetails() {
    displayPrisonerTable("Prisoner list");

    cout << "\n\t\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::manageAttendance() {
    char choice;

    do {
        displayPrisonerTable("Prisoner attendance maintainer");

        // Check if there are any prisoners
        bool hasPrisoners = false;
        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                hasPrisoners = true;
                break;
            }
        }

        if (!hasPrisoners) {
            cout << "\n\t\t\t\tNo prisoners present!\n";
            cout << "\t\t\t\tPress any key to return...";
            _getch();
            return;
        }

        int cellNumber;
        cout << "\n\t\t\t\tEnter the cell block of the prisoner: ";
        cin >> cellNumber;

        int index = getCellIndexFromNumber(cellNumber);
        if (index >= 0 && index < MAX_PRISONERS && prisoners[index].isCell()) {
            cout << "\n\t\t\t\tName: " << prisoners[index].getFullName();
            prisoners[index].decreasePunishment();
            cout << "\n\t\t\t\tPunishment for: " << prisoners[index].getPunishmentMonths() << " months";

            if (prisoners[index].isReadyForRelease()) {
                cout << "\n\t\t\t\tPrisoner is ready for release";
            }
        }
        else {
            cout << "\n\t\t\t\tInvalid cell number or empty cell!";
        }

        cout << "\n\n\t\t\t\tPress 'y' to choose another prisoner or any other key to exit: ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "\n\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::releasePrisoner() {
    char choice;

    do {
        displayPrisonerTable("Release prisoner");

        // Check if there are any prisoners
        bool hasPrisoners = false;
        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                hasPrisoners = true;
                break;
            }
        }

        if (!hasPrisoners) {
            cout << "\n\t\t\t\tNo prisoners present!\n";
            cout << "\t\t\t\tPress any key to return...";
            _getch();
            return;
        }

        int cellNumber;
        cout << "\n\t\t\t\tEnter the cell block of the prisoner: ";
        cin >> cellNumber;

        int index = getCellIndexFromNumber(cellNumber);
        if (index >= 0 && index < MAX_PRISONERS && prisoners[index].isCell()) {
            cout << "\n\t\t\t\tName: " << prisoners[index].getFullName();

            if (prisoners[index].isReadyForRelease()) {
                cout << "\n\t\t\t\tPrisoner is ready for release";
                cout << "\n\t\t\t\tPress 1 to release the prisoner: ";
                int releaseChoice;
                cin >> releaseChoice;

                if (releaseChoice == 1) {
                    prisoners[index].clearData();
                    displayLoadingAnimation("Processing", 7);
                    cout << "\n\t\t\t\tPrisoner released successfully";
                }
            }
            else {
                cout << "\n\t\t\t\tPrisoner is not ready for release";
            }
        }
        else {
            cout << "\n\t\t\t\tInvalid cell number or empty cell!";
        }

        cout << "\n\n\t\t\t\tPress 'y' to choose another prisoner or any other key to exit: ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "\n\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::searchPrisoner() {
    displayHeader("Search Menu");

    int cellNumber;
    cout << "\t\t\t\tEnter the prisoner cell id to be searched: ";
    cin >> cellNumber;

    cout << "\n===========================================================================================================\n";
    cout << "NAME\t\t\tCell Block\tAge\tGender\tHeight\tEye Colour\tCrime\tPunishment\n";
    cout << "===========================================================================================================\n";

    int index = getCellIndexFromNumber(cellNumber);
    if (index >= 0 && index < MAX_PRISONERS) {
        if (prisoners[index].isCell()) {
            cout << prisoners[index].getFullName() << "\t\t"
                << prisoners[index].getCellNo() << "\t\t"
                << prisoners[index].getAge() << "\t"
                << prisoners[index].getGender() << "\t"
                << prisoners[index].getHeight() << "\t"
                << prisoners[index].getEyeColor() << "\t\t"
                << prisoners[index].getCrime() << "\t\t"
                << prisoners[index].getPunishmentMonths() << endl;
        }
        else {
            cout << "\n\t\t\t\tCell block empty" << endl;
        }
    }
    else {
        cout << "\n\t\t\t\tInvalid cell id" << endl;
    }

    cout << "\n\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::exportPrisonData() {
    displayHeader("Export Prison Data");

    cout << "\n\n\n";
    cout << "\t\t\t\t-----------------------------------------------------------------\n";
    cout << "\t\t\t\t|                                                               |\n";
    cout << "\t\t\t\t|                     1)   Txt File                             |\n";
    cout << "\t\t\t\t|                                                               |\n";
    cout << "\t\t\t\t|                     2)   Html File                            |\n";
    cout << "\t\t\t\t|                                                               |\n";
    cout << "\t\t\t\t|                     3)   Word File                            |\n";
    cout << "\t\t\t\t|                                                               |\n";
    cout << "\t\t\t\t-----------------------------------------------------------------\n";

    int format;
    cout << "\n\t\t\t\tIn which format you want to export your file? ";
    cin >> format;

    ofstream outputFile;
    string fileName;

    switch (format) {
    case 1: // TXT format
        fileName = "Prison_Data.txt";
        outputFile.open(fileName);

        outputFile << "\t\t\t\t    -----------------------------------------------------\n";
        outputFile << "\t\t\t\t    |                                                   |\n";
        outputFile << "\t\t\t\t    |                Prisoner list                      |\n";
        outputFile << "\t\t\t\t    |                                                   |\n";
        outputFile << "\t\t\t\t    -----------------------------------------------------\n";

        outputFile << "\n=====================================================================================================================\n";
        outputFile << "SR.\tNAME\t\t\tCell Block\tAge\t\tGender\tHeight\tEye Colour\tCrime\tPunishment\n";
        outputFile << "=======================================================================================================================\n";

        for (int i = 0, count = 0; i < MAX_PRISONERS; i++) {
            if (prisoners[i].isCell()) {
                count++;
                outputFile << count << "\t"
                    << prisoners[i].getFullName() << "\t\t"
                    << prisoners[i].getCellNo() << "\t"
                    << prisoners[i].getAge() << "\t\t"
                    << prisoners[i].getGender() << "\t"
                    << prisoners[i].getHeight() << "\t\t"
                    << prisoners[i].getEyeColor() << "\t\t"
                    << prisoners[i].getCrime() << "\t\t"
                    << prisoners[i].getPunishmentMonths() << endl;
                outputFile << "\n" << endl;
            }
        }

        outputFile.close();
        displayHeader("Txt File created successfully");
        break;

    case 2: // HTML format
        fileName = "Prison_Data.html";
        outputFile.open(fileName);

        outputFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>Prison Data</title>\n";
        outputFile << "<style>table { border-collapse: collapse; width: 100%; }\n";
        outputFile << "th, td { border: 1px solid black; padding: 8px; text-align: left; }\n";
        outputFile << "th { background-color: #f2f2f2; }</style>\n</head>\n<body>\n\n";
        outputFile << "<h2>Prison Station Management System - Prisoner List</h2>\n";
        outputFile << "<table>\n<tr>\n";
        outputFile << "<th>Name</th>\n<th>Cell Block</th>\n<th>Age</th>\n<th>Gender</th>\n";
        outputFile << "<th>Height</th>\n<th>Eye Color</th>\n<th>Crime</th>\n<th>Punishment</th>\n</tr>\n";

        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                outputFile << "<tr>\n";
                outputFile << "<td>" << prisoner.getFullName() << "</td>\n";
                outputFile << "<td>" << prisoner.getCellNo() << "</td>\n";
                outputFile << "<td>" << prisoner.getAge() << "</td>\n";
                outputFile << "<td>" << prisoner.getGender() << "</td>\n";
                outputFile << "<td>" << prisoner.getHeight() << "</td>\n";
                outputFile << "<td>" << prisoner.getEyeColor() << "</td>\n";
                outputFile << "<td>" << prisoner.getCrime() << "</td>\n";
                outputFile << "<td>" << prisoner.getPunishmentMonths() << "</td>\n";
                outputFile << "</tr>\n";
            }
        }

        outputFile << "</table>\n</body>\n</html>";
        outputFile.close();
        displayHeader("Html File created successfully");
        break;

    case 3: // Word format (DOC)
        fileName = "Prison_Data.doc";
        outputFile.open(fileName);

        outputFile << "\t\t\t *** Prisoner list *** \n\n";

        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                outputFile << "First name: " << prisoner.getFirstName() << "\n";
                outputFile << "Second name: " << prisoner.getSecondName() << "\n";
                outputFile << "Cell number: " << prisoner.getCellNo() << "\n";
                outputFile << "Age: " << prisoner.getAge() << "\n";
                outputFile << "Gender: " << prisoner.getGender() << "\n";
                outputFile << "Height: " << prisoner.getHeight() << "\n";
                outputFile << "Eye colour: " << prisoner.getEyeColor() << "\n";
                outputFile << "Crime: " << prisoner.getCrime() << "\n";
                outputFile << "Punishment Months: " << prisoner.getPunishmentMonths() << "\n\n";
            }
        }

        outputFile.close();
        displayHeader("Word File created successfully");
        break;

    default:
        cout << "\n\t\t\t\tInvalid choice!\n";
    }

    cout << "\n\t\t\t\tPress any key to return...";
    _getch();
}

void PrisonManagementSystem::exit() {
    system("cls");
    displayDateTime();
    cout << "\n\n\n\n\n\n\t\t\t\t\t\tThank you!!\n\n\n\n\n\n\t\t\t\t\t\t";
    _getch();
}

void frontPage() {
    system("cls");
    displayDateTime();

    cout << "\n";
    cout << "\t\t\t\t    ----------------------------------------\n";
    cout << "\t\t\t\t    |   PRISON STATION MANAGEMENT SYSTEM   |\n";
    cout << "\t\t\t\t    ----------------------------------------\n";

    cout << "\n\n\t\t\t\tMini Project   :   Prison Management System";
    cout << "\n\n\n\n\n\t\t\t\t";
    system("pause");

    displayLoadingAnimation("Loading", 15);
}

int main() {
    frontPage();

    PrisonManagementSystem system;
    system.login();

    return 0;
}