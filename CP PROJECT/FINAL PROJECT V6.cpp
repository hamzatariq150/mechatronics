#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
const int MAX_STUDENTS = 100;
struct Student {
    string name;
    string rollNo;
    double quizzes[10];
    int quizCount;
    double assignments[10];
    int assignCount;
    double midterm;
    double finalterm;
    double project;
    double total;
    string grade;
};
int readCSV(Student students[], string filename, int& quizCount, int& assignCount) { // to read csv file 
    ifstream file(filename);
    string line;
    int count = 0;
    bool firstLine = true;

    while (getline(file, line)) {
        stringstream ss(line);//it counts the file line
        string value;
        int column = 0;
        if (firstLine) {
            int q = 0, a = 0;
            while (getline(ss, value, ',')) {
                if (value == "Q") q++;// it gets the value of q=quizs in a line of the given file
                if (value == "A") a++;//again it counts for a in  a line
            }
            quizCount = q;
            assignCount = a;
            firstLine = false;
            continue;
        }
        ss.clear();
        ss.str(line);
        Student s;
        getline(ss, s.name, ',');
        getline(ss, s.rollNo, ',');
        for (int i = 0; i < quizCount; i++) {
            getline(ss, value, ',');
            s.quizzes[i] = stod(value);
        }
        for (int i = 0; i < assignCount; i++) {
            getline(ss, value, ',');
            s.assignments[i] = stod(value);
        }
        getline(ss, value, ',');
        s.midterm = stod(value);
        getline(ss, value, ',');
        s.finalterm = stod(value);
        getline(ss, value, ',');
        s.project = stod(value);

        s.quizCount = quizCount;
        s.assignCount = assignCount;

        students[count++] = s;
    }
    file.close();
    return count;
}

void inputWeights(double& qw, double& aw, double& mw, double& fw, double& pw) {
    double total;
    do {
        cout << "Enter weight for quizzes: ";
        cin >> qw;
        cout << "Enter weight for assignments: ";
        cin >> aw;
        cout << "Enter weight for midterm: ";
        cin >> mw;
        cout << "Enter weight for final: ";
        cin >> fw;
        cout << "Enter weight for project: ";
        cin >> pw;
        total = qw + aw + mw + fw + pw;
        if (total != 100) {
            cout << "Total must be 100. Try again.\n";
        }
    } while (total != 100);
}
double calculateTotal(Student& s, double qw, double aw, double mw, double fw, double pw) { // this functio calculate the total numbers
    double quizSum = 0, assignSum = 0;
    for (int i = 0; i < s.quizCount; i++) quizSum += s.quizzes[i];
    for (int i = 0; i < s.assignCount; i++) assignSum += s.assignments[i];

    double quizAvg = (quizSum / (s.quizCount * 10)) * qw;
    double assignAvg = (assignSum / (s.assignCount * 10)) * aw;
    double midAvg = (s.midterm / 100) * mw;
    double finalAvg = (s.finalterm / 100) * fw;
    double projAvg = (s.project / 40) * pw;

    return quizAvg + assignAvg + midAvg + finalAvg + projAvg;//this gives the average of the q,a,m,f and p.
}
double calculateClassAverage(Student students[], int count) {
    double total = 0;
    for (int i = 0; i < count; i++) {
        total += students[i].total;
    }
    return total / count;
}
string getGrade(double marks, double avg) {// here we creat a function for grade
    if (marks <= avg - 22) return "F";
    else if (marks <= avg - 18) return "D";
    else if (marks <= avg - 14) return "C-";
    else if (marks <= avg - 10) return "C";
    else if (marks <= avg - 6) return "C+";
    else if (marks <= avg - 2) return "B-";
    else if (marks <= avg + 2) return "B";
    else if (marks <= avg + 6) return "B+";
    else if (marks <= avg + 10) return "A-";
    else return "A";
}
void displayResults(Student students[], int count) {//we use void because it has no returning value
    cout << "Name\t  Roll No\t   Total\t   Grade\n";
    for (int i = 0; i < count; i++) {
        cout << students[i].name << "  \t  " << students[i].rollNo << "  \t  " << students[i].total << "  \t  " << students[i].grade << endl;
    }
}
void saveResultsToFile(Student students[], int count, string filename) {//it saves the file of the calculted result
    ofstream out(filename);
    out << "Name , RollNo , Total , Grade\n";
    for (int i = 0; i < count; i++) {
        out << students[i].name << " , " << students[i].rollNo << "," << students[i].total << "," << students[i].grade << "\n";
    }
    out.close();
    cout << "Results saved to " << filename << endl;
}
void searchStudent(Student students[], int count) {
    string roll;
    cout << "Enter roll number: ";
    cin >> roll;
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            cout << "Name: " << students[i].name << ", Total: " << students[i].total << ", Grade: " << students[i].grade << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Student not found.\n";
    }
}
int main() {
    Student students[MAX_STUDENTS];//here we recall the function for the csv file
    string filename;
    int quizCount, assignCount;
    double qw, aw, mw, fw, pw;
    cout << "Enter(.csv) filename : ";
    cin >> filename;
    int totalStudents = readCSV(students, filename, quizCount, assignCount);//it read the contents of the csv file
    inputWeights(qw, aw, mw, fw, pw);
    for (int i = 0; i < totalStudents; i++) {
        students[i].total = calculateTotal(students[i], qw, aw, mw, fw, pw);
    }
    double avg = calculateClassAverage(students, totalStudents);
    for (int i = 0; i < totalStudents; i++) {
        students[i].grade = getGrade(students[i].total, avg);
    }
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Show class result\n";
        cout << "2. Save result to file\n";
        cout << "3. Search student by roll no\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        if (choice == 1) displayResults(students, totalStudents);
        else if (choice == 2) {
            string outFile;
            cout << "Enter output file name: ";
            cin >> outFile;
            saveResultsToFile(students, totalStudents, outFile);
        }
        else if (choice == 3) searchStudent(students, totalStudents);
        else if (choice == 4) cout << "Exiting...\n";//exit
        else cout << "Invalid choice.\n";
    } while (choice != 4);
    return 0;
}
