//============================================================================
// Name        : HashTable.cpp
// Author      : Scott Cain
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Final Project Using Hash Table
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <climits>

using namespace std;

// Define a structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Define a structure for the nodes in the hash table
struct Node {
    Course course;
    unsigned int key;
    Node* next;

    Node() : key(UINT_MAX), next(nullptr) {}
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
    Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
        key = aKey;
    }
};

// Hash table class definition
class HashTable {
private:
    vector<Node> nodes;
    unsigned int tableSize;
    unsigned int hash(string key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseNumber);
    Course Search(string courseNumber);
    size_t Size();
};

unsigned int HashTable::hash(string key) {
    unsigned int hashValue = 0;
    for (char ch : key) {
        hashValue += ch;
    }
    return hashValue % tableSize;
}

HashTable::HashTable() {
    tableSize = 179;
    nodes.resize(tableSize);
}

HashTable::HashTable(unsigned int size) {
    tableSize = size;
    nodes.resize(tableSize);
}

HashTable::~HashTable() {
    for (auto& node : nodes) {
        Node* entry = node.next;
        while (entry != nullptr) {
            Node* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
}

void HashTable::Insert(Course course) {
    unsigned int key = hash(course.courseNumber);
    Node* prev = nullptr;
    Node* entry = &nodes.at(key);

    while (entry != nullptr && entry->key != UINT_MAX && entry->course.courseNumber != course.courseNumber) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr || entry->key == UINT_MAX) {
        entry = new Node(course, key);
        if (prev == nullptr) {
            nodes.at(key).next = entry;
        }
        else {
            prev->next = entry;
        }
    }
    else {
        entry->course = course;
    }
}

void HashTable::PrintAll() {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* entry = nodes.at(i).next;
        while (entry != nullptr) {
            cout << entry->course.courseNumber << ", " << entry->course.courseTitle << endl;
            entry = entry->next;
        }
    }
}

void HashTable::Remove(string courseNumber) {
    unsigned int key = hash(courseNumber);
    Node* prev = nullptr;
    Node* entry = nodes.at(key).next;

    while (entry != nullptr && entry->course.courseNumber != courseNumber) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        return;
    }

    if (prev == nullptr) {
        nodes.at(key).next = entry->next;
    }
    else {
        prev->next = entry->next;
    }

    delete entry;
}

Course HashTable::Search(string courseNumber) {
    unsigned int key = hash(courseNumber);
    Node* entry = nodes.at(key).next;

    while (entry != nullptr) {
        if (entry->course.courseNumber == courseNumber) {
            return entry->course;
        }
        entry = entry->next;
    }

    return Course();
}

// Function to load courses from a file
void loadCourses(string filename, HashTable& table) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        string courseNumber, courseTitle, prereq;
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        Course course = { courseNumber, courseTitle };
        while (getline(ss, prereq, ',')) {
            if (!prereq.empty()) {
                course.prerequisites.push_back(prereq);
            }
        }
        table.Insert(course);
    }
}

// Function to display the menu
void displayMenu() {
    cout << "1. Load Data Structure.\n";
    cout << "2. Print Course List.\n";
    cout << "3. Print Course.\n";
    cout << "9. Exit\n";
}

// Function to print course information
void printCourseInfo(HashTable& table, const string& courseNumber) {
    Course course = table.Search(courseNumber);
    if (course.courseNumber.empty()) {
        cerr << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << "\nPrerequisites: ";
    for (const auto& prereq : course.prerequisites) {
        cout << prereq << " ";
    }
    cout << endl;
}

// Main function
int main() {
    int choice;
    HashTable courses;

    cout << "Welcome to the course planner.\n";

    while (true) {
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            string filename;
            cout << "Enter file name: ";
            cin >> filename;
            loadCourses(filename, courses);
            cout << "Data loaded successfully.\n";
        }
        else if (choice == 2) {
            courses.PrintAll();
        }
        else if (choice == 3) {
            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            printCourseInfo(courses, courseNumber);
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!\n";
            break;
        }
        else {
            cerr << choice << " is not a valid option.\n";
        }
    }

    return 0;
}
