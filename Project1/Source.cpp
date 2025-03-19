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
#include <algorithm>
#include <functional>
#include <memory>

using namespace std;

// Constants
namespace Constants {
    const int MAX_PRISONERS = 20;
    const int BASE_CELL_NUMBER = 1001;
    const int MAX_LOGIN_ATTEMPTS = 3;
}

// Utility functions
class Utils {
public:
    static void displayDateTime() {
        time_t currentTime = time(nullptr);
        tm timeInfo;
		localtime_s(&timeInfo, &currentTime);

        cout << "Time: " << setfill('0') << setw(2) << timeInfo.tm_hour << ":"
            << setfill('0') << setw(2) << timeInfo.tm_min << ":"
            << setfill('0') << setw(2) << timeInfo.tm_sec << endl;

        cout << "Date: " << timeInfo.tm_mday << "/"
            << timeInfo.tm_mon + 1 << "/" << timeInfo.tm_year + 1900 << "\n";
    }

    static void displayLoadingAnimation(const string& message, int numDots = 15) {
        cout << "\n\t\t\t\t" << message;
        for (int i = 0; i < numDots; i++) {
            Sleep(30);
            cout << ".";
        }
        cout << endl;
    }

    static void displayHeader(const string& title) {
        system("cls");
        displayDateTime();

        cout << "\t\t\t\t    -----------------------------------------------------\n";
        cout << "\t\t\t\t    |                                                   |\n";

        int total_width = 51;
        int text_width = title.length();
        int left_padding = (total_width - text_width) / 2;
        int right_padding = total_width - text_width - left_padding;

        cout << "\t\t\t\t    |" << setfill(' ') << setw(left_padding) << " "
            << title
            << setfill(' ') << setw(right_padding) << " " << "|\n";

        cout << "\t\t\t\t    |                                                   |\n";
        cout << "\t\t\t\t    -----------------------------------------------------\n\n";
    }

    static void pause() {
        cout << "\n\t\t\t\tPress any key to continue...";
        _getch();
    }

    static void clearScreen() {
        system("cls");
    }

    static string securePasswordInput() {
        string password;
        char ch;

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

        return password;
    }
};

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
    explicit Prisoner(int cellId = 0) :
        firstName(""), secondName(""), gender(""),
        cellNo(Constants::BASE_CELL_NUMBER + cellId),
        age(0), height(0.0), eyeColor(""), crime(""),
        punishmentMonths(0), isOccupied(false) {}

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
    void decreasePunishment() {
        if (punishmentMonths > 0) {
            punishmentMonths--;
        }
    }

    bool isReadyForRelease() const { return punishmentMonths <= 0; }

    void clearData() {
        firstName.clear();
        secondName.clear();
        gender.clear();
        age = 0;
        height = 0.0;
        eyeColor.clear();
        crime.clear();
        punishmentMonths = 0;
        isOccupied = false;
    }
};

// Data export formats
class DataExporter {
public:
    virtual ~DataExporter() = default;
    virtual void exportData(const vector<Prisoner>& prisoners, const string& fileName) const = 0;
};

class TxtExporter : public DataExporter {
public:
    void exportData(const vector<Prisoner>& prisoners, const string& fileName) const override {
        ofstream outputFile(fileName);
        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        outputFile << "\t\t\t\t    -----------------------------------------------------\n";
        outputFile << "\t\t\t\t    |                                                   |\n";
        outputFile << "\t\t\t\t    |                Prisoner list                      |\n";
        outputFile << "\t\t\t\t    |                                                   |\n";
        outputFile << "\t\t\t\t    -----------------------------------------------------\n";

        outputFile << "\n=====================================================================================================================\n";
        outputFile << "SR.\tNAME\t\t\tCell Block\tAge\t\tGender\tHeight\tEye Colour\tCrime\tPunishment\n";
        outputFile << "=======================================================================================================================\n";

        int count = 0;
        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                count++;
                outputFile << count << "\t"
                    << prisoner.getFullName() << "\t\t"
                    << prisoner.getCellNo() << "\t"
                    << prisoner.getAge() << "\t\t"
                    << prisoner.getGender() << "\t"
                    << prisoner.getHeight() << "\t\t"
                    << prisoner.getEyeColor() << "\t\t"
                    << prisoner.getCrime() << "\t\t"
                    << prisoner.getPunishmentMonths() << endl;
                outputFile << "\n" << endl;
            }
        }

        outputFile.close();
    }
};

class HtmlExporter : public DataExporter {
public:
    void exportData(const vector<Prisoner>& prisoners, const string& fileName) const override {
        ofstream outputFile(fileName);
        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

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
    }
};

class DocExporter : public DataExporter {
public:
    void exportData(const vector<Prisoner>& prisoners, const string& fileName) const override {
        ofstream outputFile(fileName);
        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

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
    }
};

// Factory for creating exporters
class ExporterFactory {
public:
    static unique_ptr<DataExporter> createExporter(int formatChoice) {
        switch (formatChoice) {
        case 1: return make_unique<TxtExporter>();
        case 2: return make_unique<HtmlExporter>();
        case 3: return make_unique<DocExporter>();
        default: return nullptr;
        }
    }
};

// Authentication Manager
class AuthManager {
private:
    bool isLoggedIn;

public:
    AuthManager() : isLoggedIn(false) {}

    bool login() {
        string username, password;
        int attempts = 0;

        while (attempts < Constants::MAX_LOGIN_ATTEMPTS) {
            Utils::clearScreen();
            Utils::displayDateTime();

            cout << "\n\n\n\n\n\n";
            cout << "\t\t\t\t    ----------------------------------------\n";
            cout << "\t\t\t\t    |   PRISON STATION MANAGEMENT SYSTEM   |\n";
            cout << "\t\t\t\t    ----------------------------------------\n\n\n";

            cout << "\t\t\t\t\tUsername: ";
            cin >> username;

            cout << "\n\t\t\t\t\tPassword: ";
            password = Utils::securePasswordInput();

            if (validateCredentials(username, password)) {
                cout << "\n\n\t\t\t\tYou are accessed to the system!\n\n";
                cout << "\t\t\t\t";
				Utils::pause();
                isLoggedIn = true;
                return true;
            }
            else {
                attempts++;
                Utils::clearScreen();
                cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
                cout << "\t\t\t\t\tNo. of attempts remain: "
                    << Constants::MAX_LOGIN_ATTEMPTS - attempts;
                cout << "\n\n\t\t\t\t";
                Utils::pause();
            }
        }

        cout << "\n\t\t\t\tNo permission to enter the system!\n\n";
        cout << "\t\t\t\t";
        Utils::pause();
        return false;
    }

    void logout() {
        isLoggedIn = false;
    }

    bool isAuthenticated() const {
        return isLoggedIn;
    }

private:
    bool validateCredentials(const string& username, const string& password) const {
        return (username == "admin" && password == "pass");
    }
};

// Prison Management System class
class PrisonManagementSystem {
private:
    vector<Prisoner> prisoners;
    AuthManager authManager;

    // Helper methods
    void displayPrisonerTable(const string& title) const {
        Utils::displayHeader(title);

        cout << "\n===========================================================================================================\n";
        cout << left << setw(20) << "NAME"
            << setw(15) << "Cell Block"
            << setw(8) << "Age"
            << setw(10) << "Gender"
            << setw(10) << "Height"
            << setw(15) << "Eye Colour"
            << setw(15) << "Crime"
            << "Punishment\n";
        cout << "===========================================================================================================\n";
        int count = 0;
        for (const auto& prisoner : prisoners) {
            if (prisoner.isCell()) {
                cout << left << setw(20) << prisoner.getFullName()
                    << setw(15) << prisoner.getCellNo()
                    << setw(8) << prisoner.getAge()
                    << setw(10) << prisoner.getGender()
                    << setw(10) << prisoner.getHeight()
                    << setw(15) << prisoner.getEyeColor()
                    << setw(15) << prisoner.getCrime()
                    << prisoner.getPunishmentMonths() << endl;
                cout << "\n";
                count++;
            }
        }

        if (count == 0) {
            cout << "\n\t\t\t\tNo prisoners present\n";
        }
    }

    void displayMenu() const {
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

    int getCellIndexFromNumber(int cellNumber) const {
        return cellNumber - Constants::BASE_CELL_NUMBER;
    }

    int findFreeCellIndex() const {
        for (size_t i = 0; i < prisoners.size(); i++) {
            if (!prisoners[i].isCell()) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    bool hasPrisoners() const {
        return any_of(prisoners.begin(), prisoners.end(),
            [](const Prisoner& p) { return p.isCell(); });
    }

    bool isValidCellIndex(int index) const {
        return index >= 0 && index < static_cast<int>(prisoners.size()) && prisoners[index].isCell();
    }

    template <typename Action>
    void processPrisonerAction(const Action& action) {
        char choice;
        do {
            displayPrisonerTable("Prisoner Management");

            if (!hasPrisoners()) {
                Utils::pause();
                return;
            }

            int cellNumber;
            cout << "\n\t\t\t\tEnter the cell block of the prisoner: ";
            cin >> cellNumber;

            int index = getCellIndexFromNumber(cellNumber);
            if (isValidCellIndex(index)) {
                cout << "\n\t\t\t\tName: " << prisoners[index].getFullName();
                action(index);
            }
            else {
                cout << "\n\t\t\t\tInvalid cell number or empty cell!";
            }

            cout << "\n\n\t\t\t\tPress 'y' to choose another prisoner or any other key to exit: ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');

        Utils::pause();
    }

public:
    // Constructor
    PrisonManagementSystem() {
        // Initialize prisoners with cell numbers
        prisoners.resize(Constants::MAX_PRISONERS);
        for (int i = 0; i < Constants::MAX_PRISONERS; i++) {
            prisoners[i] = Prisoner(i);
        }
    }

    // Main functions
    void run() {
        frontPage();

        if (authManager.login()) {
            mainMenu();
        }
    }

    void frontPage() const {
        Utils::clearScreen();
        Utils::displayDateTime();

        cout << "\n";
        cout << "\t\t\t\t    ----------------------------------------\n";
        cout << "\t\t\t\t    |   PRISON STATION MANAGEMENT SYSTEM   |\n";
        cout << "\t\t\t\t    ----------------------------------------\n";

        cout << "\n\n\t\t\t\tMini Project   :   Prison Management System";
        cout << "\n\n\n\n\n\t\t\t\t";
        Utils::pause();

        Utils::displayLoadingAnimation("Loading", 15);
    }

    void mainMenu() {
        int choice;

        do {
            Utils::clearScreen();
            Utils::displayDateTime();
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
                Utils::pause();
            }
        } while (true);
    }

    void addPrisoner() {
        Utils::displayHeader("Enter basic information of prisoner");

        // Find an empty cell
        int index = findFreeCellIndex();

        if (index == -1) {
            cout << "\n\t\t\t\tNo empty cells available!\n";
            Utils::pause();
            return;
        }

        string firstName, secondName, gender, eyeColor, crime;
        int age, punishmentMonths;
        double height;

        cout << "\t\t\t\t\tEnter first name: ";
        cin >> firstName;
        cout << "\n\t\t\t\t\tEnter second name: ";
        cin >> secondName;
        cout << "\n\t\t\t\t\tEnter gender: ";
        cin >> gender;
        cout << "\n\t\t\t\t\tEnter age: ";
        cin >> age;
        cout << "\n\t\t\t\t\tEnter height: ";
        cin >> height;
        cout << "\n\t\t\t\t\tEnter eye color: ";
        cin >> eyeColor;
        cout << "\n\t\t\t\t\tEnter crime: ";
        cin >> crime;
        cout << "\n\t\t\t\t\tEnter punishment span in months: ";
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
        Utils::pause();
    }

    void showPrisonerDetails() const {
        displayPrisonerTable("Prisoner list");
        Utils::pause();
    }

    void manageAttendance() {
        processPrisonerAction([this](int index) {
            prisoners[index].decreasePunishment();
            cout << "\n\t\t\t\tPunishment for: " << prisoners[index].getPunishmentMonths() << " months";

            if (prisoners[index].isReadyForRelease()) {
                cout << "\n\t\t\t\tPrisoner is ready for release";
            }
            });
    }

    void releasePrisoner() {
        processPrisonerAction([this](int index) {
            if (prisoners[index].isReadyForRelease()) {
                cout << "\n\t\t\t\tPrisoner is ready for release";
                cout << "\n\t\t\t\tPress 1 to release the prisoner: ";
                int releaseChoice;
                cin >> releaseChoice;

                if (releaseChoice == 1) {
                    prisoners[index].clearData();
                    Utils::displayLoadingAnimation("Processing", 7);
                    cout << "\n\t\t\t\tPrisoner released successfully";
                }
            }
            else {
                cout << "\n\t\t\t\tPrisoner is not ready for release";
            }
            });
    }

    void searchPrisoner() {
        Utils::displayHeader("Search Menu");

        int cellNumber;
        cout << "\t\t\t\tEnter the prisoner cell id to be searched: ";
        cin >> cellNumber;

        cout << "\n===========================================================================================================\n";
        cout << "NAME\t\t\tCell Block\tAge\tGender\tHeight\tEye Colour\tCrime\tPunishment\n";
        cout << "===========================================================================================================\n";

        int index = getCellIndexFromNumber(cellNumber);
        if (index >= 0 && index < static_cast<int>(prisoners.size())) {
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

        Utils::pause();
    }

    void exportPrisonData() const {
        Utils::displayHeader("Export Prison Data");

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

        unique_ptr<DataExporter> exporter = ExporterFactory::createExporter(format);

        if (exporter) {
            string fileName;

            switch (format) {
            case 1: fileName = "Prison_Data.txt"; break;
            case 2: fileName = "Prison_Data.html"; break;
            case 3: fileName = "Prison_Data.doc"; break;
            default: cout << "\n\t\t\t\tInvalid format!"; Utils::pause(); return;
            }

            exporter->exportData(prisoners, fileName);

            string successTitle;
            switch (format) {
            case 1: successTitle = "Txt File created successfully"; break;
            case 2: successTitle = "Html File created successfully"; break;
            case 3: successTitle = "Word File created successfully"; break;
            }

            Utils::displayHeader(successTitle);
        }
        else {
            cout << "\n\t\t\t\tInvalid choice!\n";
        }

        Utils::pause();
    }

    void logout() {
        Utils::clearScreen();
        Utils::displayLoadingAnimation("Logging out", 15);
        authManager.logout();
    }

    void exit() const {
        Utils::clearScreen();
        Utils::displayDateTime();
        cout << "\n\n\n\n\n\n\t\t\t\t\t\tThank you!!\n\n\n\n\n\n\t\t\t\t\t\t";
        _getch();
    }
};

int main() {
    PrisonManagementSystem system;
    system.run();
    return 0;
}