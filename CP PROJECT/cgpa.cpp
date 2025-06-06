#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Function to convert grade to points
float gradeToPoint(string grade) {
    if (grade == "A") return 4.0;
    if (grade == "A-") return 3.7;
    if (grade == "B+") return 3.3;
    if (grade == "B") return 3.0;
    if (grade == "B-") return 2.7;
    if (grade == "C+") return 2.3;
    if (grade == "C") return 2.0;
    if (grade == "C-") return 1.7;
    if (grade == "D") return 1.0;
    return 0.0; // F or anything else
}

// Structure to hold student data
struct Student {
    string name;
    string rollNo;
    float totalPoints = 0;
    int totalCredits = 0;
};

const int MAX_STUDENTS = 100;
Student students[MAX_STUDENTS];
int studentCount = 0;

// Function to find student index by roll number
int findStudent(string rollNo) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].rollNo == rollNo) {
            return i;
        }
    }
    return -1;
}

// Function to process subject file
void processSubjectFile() {
    string fileName;
    int credit;
    cout << "Enter subject result file name (e.g., cp_result.csv): ";
    cin >> fileName;
    cout << "Enter credit hours for this subject: ";
    cin >> credit;

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could not open file: " << fileName << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string name, rollNo, totalStr, grade;
        getline(ss, name, ',');
        getline(ss, rollNo, ',');
        getline(ss, totalStr, ',');
        getline(ss, grade, ',');

        float points = gradeToPoint(grade) * credit;

        int idx = findStudent(rollNo);
        if (idx == -1) {
            idx = studentCount++;
            students[idx].name = name;
            students[idx].rollNo = rollNo;
        }
        students[idx].totalPoints += points;
        students[idx].totalCredits += credit;
    }

    file.close();
    cout << "Subject file processed successfully!\n";
}

// Function to display CGPA of all students
void displayCGPA() {
    cout << "\nCGPA of Students:\n";
    cout << "-----------------------------\n";
    cout << "Roll No\t\tName\t\tCGPA\n";
    cout << "-----------------------------\n";

    for (int i = 0; i < studentCount; i++) {
        float cgpa = students[i].totalPoints / students[i].totalCredits;
        cout << students[i].rollNo << "\t" << students[i].name << "\t" << cgpa << endl;
    }
}

// Function to save CGPA to file
void saveCGPAToFile() {
    ofstream outFile("cgpa_results.csv");
    outFile << "Name,Roll No,CGPA\n";

    for (int i = 0; i < studentCount; i++) {
        float cgpa = students[i].totalPoints / students[i].totalCredits;
        outFile << students[i].name << "," << students[i].rollNo << "," << cgpa << "\n";
    }

    outFile.close();
    cout << "CGPA saved to cgpa_results.csv successfully!\n";
}

int main() {
    int choice;

    do {
        cout << "\n===== CGPA MENU =====\n";
        cout << "1. Add Subject File\n";
        cout << "2. Show CGPA of Students\n";
        cout << "3. Save CGPA to File\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            processSubjectFile();
        } else if (choice == 2) {
            displayCGPA();
        } else if (choice == 3) {
            saveCGPAToFile();
        } else if (choice == 4) {
            cout << "Exiting...\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
