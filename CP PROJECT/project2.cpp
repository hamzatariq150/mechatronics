#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath> 

using namespace std;

// Structure to store student data
struct Student {
    string name;
    string rollNo;
    vector<int> quizzes;
    vector<int> assignments;
    int midterm, finalExam, project;
    double totalMarks;
    string grade;
};

// Function to calculate average
double getAverage(const vector<int>& scores) {
    double sum = 0;
    for (int score : scores) sum += score;
    return scores.empty() ? 0 : sum / scores.size();
}

// Function to assign grade based on class average
string assignGrade(double marks, double avg) {
    int roundedAvg = round(avg);

    if (marks <= roundedAvg - 23) return "F";
    else if (marks <= roundedAvg - 18) return "D";
    else if (marks <= roundedAvg - 13) return "C-";
    else if (marks <= roundedAvg - 8)  return "C";
    else if (marks <= roundedAvg - 3)  return "C+";
    else if (marks <= roundedAvg + 2)  return "B";
    else if (marks <= roundedAvg + 7)  return "B+";
    else if (marks <= roundedAvg + 12) return "A-";
    else return "A";
}

int main() {
    string fileName;
    cout << "Enter file name (e.g., NCA.csv): ";
    cin >> fileName;

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    string line;
    vector<string> headers;
    getline(file, line); // Read header

    stringstream ss(line);
    string word;
    while (getline(ss, word, ',')) headers.push_back(word);

    int numQuizzes = 0, numAssignments = 0;
    for (string h : headers) {
        if (!h.empty() && h[0] == 'Q') numQuizzes++;
        if (!h.empty() && h[0] == 'A') numAssignments++;
    }

    vector<Student> students;

    while (getline(file, line)) {
        stringstream lineStream(line);
        Student s;
        string cell;

        getline(lineStream, s.name, ',');
        getline(lineStream, s.rollNo, ',');

        for (int i = 0; i < numQuizzes; i++) {
            getline(lineStream, cell, ',');
            s.quizzes.push_back(stoi(cell));
        }

        for (int i = 0; i < numAssignments; i++) {
            getline(lineStream, cell, ',');
            s.assignments.push_back(stoi(cell));
        }

        getline(lineStream, cell, ','); s.midterm = stoi(cell);
        getline(lineStream, cell, ','); s.finalExam = stoi(cell);
        getline(lineStream, cell, ','); s.project = stoi(cell);

        students.push_back(s);
    }

    file.close();

    // Get weightages
    double wQuiz, wAssign, wMid, wFinal, wProject;
    do {
        cout << "\nEnter weightages (total must be 100):\n";
        cout << "Quiz: "; cin >> wQuiz;
        cout << "Assignment: "; cin >> wAssign;
        cout << "Midterm: "; cin >> wMid;
        cout << "Final: "; cin >> wFinal;
        cout << "Project: "; cin >> wProject;

        if (wQuiz + wAssign + wMid + wFinal + wProject != 100)
            cout << "Invalid! Total must be 100.\n";
    } while (wQuiz + wAssign + wMid + wFinal + wProject != 100);

    // Calculate total marks
    double totalOfAll = 0;
    for (auto& s : students) {
        double qAvg = getAverage(s.quizzes) / 10.0;
        double aAvg = getAverage(s.assignments) / 10.0;
        double m = s.midterm / 100.0;
        double f = s.finalExam / 100.0;
        double p = s.project / 40.0;

        s.totalMarks = qAvg * wQuiz + aAvg * wAssign + m * wMid + f * wFinal + p * wProject;
        totalOfAll += s.totalMarks;
    }

    double classAverage = totalOfAll / students.size();

    // Assign grades
    for (auto& s : students)
        s.grade = assignGrade(s.totalMarks, classAverage);

    // Menu
    int option;
    do {
        cout << "\nMenu:\n";
        cout << "1. Show class results\n";
        cout << "2. Save results to CSV\n";
        cout << "3. Search by roll number\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> option;

        if (option == 1) {
            cout << left << setw(25) << "Name" << setw(15) << "Roll No"
                 << setw(10) << "Marks" << setw(6) << "Grade\n";
            for (auto& s : students) {
                cout << setw(25) << s.name << setw(15) << s.rollNo
                     << setw(10) << fixed << setprecision(2) << s.totalMarks
                     << setw(6) << s.grade << "\n";
            }
        } else if (option == 2) {
            string outFile = fileName.substr(0, fileName.find('.')) + "_result.csv";
            ofstream fout(outFile);
            fout << "Name,Roll No,Marks,Grade\n";
            for (auto& s : students)
                fout << s.name << "," << s.rollNo << "," << fixed << setprecision(2)
                     << s.totalMarks << "," << s.grade << "\n";
            fout.close();
            cout << "Results saved to " << outFile << "\n";
        } else if (option == 3) {
            string roll;
            cout << "Enter roll number: ";
            cin >> roll;
            bool found = false;
            for (auto& s : students) {
                if (s.rollNo == roll) {
                    cout << "Name: " << s.name << ", Marks: " << s.totalMarks
                         << ", Grade: " << s.grade << "\n";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Student not found.\n";
        }

    } while (option != 4);

    return 0;
}
