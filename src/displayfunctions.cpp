#include "../include/displayfunctions.hpp"
#include <iostream>
#include <sqlite3.h>
#include <algorithm>
#include <limits>
#include "Assignments_DB.hpp" 

const std::string dbName = "assignments.db"; // Database name

// Display all assignments: 
void DisplayFunctions::displayAllAssignments(const std::vector<AssignmentPtr>&) {
    Database::showAllAssignments(dbName);
}

// Display assignments filtered by subject:
void DisplayFunctions::displayAssignmentsBySubject(const std::vector<AssignmentPtr>&, const std::string& subject) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Assignments WHERE subject = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_STATIC);

        bool found = false;
        std::cout << "\nAssignments for Subject: " << subject << "\n";
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        std::cout << " ID | Subject | Name | Deadline | Duration | Weight | Size | Group Work | Group Size" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            found = true;
            std::cout << sqlite3_column_int(stmt, 0) << " | ";
            std::cout << sqlite3_column_text(stmt, 1) << " | ";
            std::cout << sqlite3_column_text(stmt, 2) << " | ";
            std::cout << sqlite3_column_int(stmt, 3) << " | ";
            std::cout << sqlite3_column_int(stmt, 4) << " | ";
            std::cout << sqlite3_column_double(stmt, 5) << " | ";
            std::cout << sqlite3_column_int(stmt, 6) << " | ";
            std::cout << (sqlite3_column_int(stmt, 7) ? "Yes" : "No") << " | ";
            std::cout << sqlite3_column_int(stmt, 8) << std::endl;
        }

        if (!found) {
            std::cout << "No assignments found for subject: " << subject << "\n";
        }

        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Error querying assignments by subject: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Display assignments sorted by shortest deadline:
void DisplayFunctions::displayAssignmentsByShortestDeadline(const std::vector<AssignmentPtr>&) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Assignments ORDER BY deadline ASC;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "\nAssignments by Shortest Deadline:\n";
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        std::cout << " ID | Subject | Name | Deadline | Duration | Weight | Size | Group Work | Group Size" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(stmt, 0) << " | ";
            std::cout << sqlite3_column_text(stmt, 1) << " | ";
            std::cout << sqlite3_column_text(stmt, 2) << " | ";
            std::cout << sqlite3_column_int(stmt, 3) << " | ";
            std::cout << sqlite3_column_int(stmt, 4) << " | ";
            std::cout << sqlite3_column_double(stmt, 5) << " | ";
            std::cout << sqlite3_column_int(stmt, 6) << " | ";
            std::cout << (sqlite3_column_int(stmt, 7) ? "Yes" : "No") << " | ";
            std::cout << sqlite3_column_int(stmt, 8) << std::endl;
        }

        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Error querying assignments by deadline: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Display assignments sorted by biggest duration:
void DisplayFunctions::displayAssignmentsByBiggestDuration(const std::vector<AssignmentPtr>&) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Assignments ORDER BY duration DESC;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "\nAssignments by Biggest Duration:\n";
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        std::cout << " ID | Subject | Name | Deadline | Duration | Weight | Size | Group Work | Group Size" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(stmt, 0) << " | ";
            std::cout << sqlite3_column_text(stmt, 1) << " | ";
            std::cout << sqlite3_column_text(stmt, 2) << " | ";
            std::cout << sqlite3_column_int(stmt, 3) << " | ";
            std::cout << sqlite3_column_int(stmt, 4) << " | ";
            std::cout << sqlite3_column_double(stmt, 5) << " | ";
            std::cout << sqlite3_column_int(stmt, 6) << " | ";
            std::cout << (sqlite3_column_int(stmt, 7) ? "Yes" : "No") << " | ";
            std::cout << sqlite3_column_int(stmt, 8) << std::endl;
        }

        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Error querying assignments by duration: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Display menu options for assignments:
void DisplayFunctions::displayMenu(const std::vector<AssignmentPtr>& assignments) {
    while (true) {
        std::cout << "\nDisplay Menu:\n"
                  << "1. Display all assignments\n"
                  << "2. Display assignments by subject\n"
                  << "3. Display assignments by shortest deadline\n"
                  << "4. Display assignments by biggest duration\n"
                  << "5. Go back\n"
                  << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        // Input validation:
        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        switch (choice) {
            case 1:
                displayAllAssignments(assignments);
                break;
            case 2: {
                std::cout << "Enter the subject: ";
                std::string subject;
                std::cin >> subject;
                displayAssignmentsBySubject(assignments, subject);
                break;
            }
            case 3:
                displayAssignmentsByShortestDeadline(assignments);
                break;
            case 4:
                displayAssignmentsByBiggestDuration(assignments);
                break;
            case 5:
                std::cout << "Exiting display menu.\n";
                return;
        }
    }
}