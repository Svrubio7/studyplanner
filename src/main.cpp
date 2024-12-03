#include <iostream>
#include <limits>
#include <string>
#include <sqlite3.h>
#include "../include/assignment.hpp"
#include "../include/planner.hpp"
#include "../include/displayfunctions.hpp"
#include "Assignments_DB.hpp"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

// Ensure the Data directory exists:
void ensureDataDirectoryExists() {
    if (!fs::exists("Data")) {
        if (!fs::create_directory("Data")) {
            std::cerr << "Error: Could not create 'Data' directory.\n";
            exit(1); // Exit if unable to create the directory
        }
    }
}

int main() {
    const std::string dbName = "assignments.db";

    // Ensure the Data directory exists:
    ensureDataDirectoryExists();

    // Initialize the database and create the Assignments table:
    Database::initialize(dbName);
    Database::createAssignmentTable(dbName);

    // Greeting username and welcome message:
    std::cout << "Enter your name: ";
    std::string name;
    std::cin >> name;

    std::cout << "Welcome, " << name << "!\n";

    // Main menu loop:
    while (true) {
        std::cout << "\nMain Menu:\n";
        std::cout << "1. Add an Assignment\n";
        std::cout << "2. Delete an Assignment\n";
        std::cout << "3. Run Priority-Based Scheduler\n";
        std::cout << "4. Display Options\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        // Input validation:
        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear(); // Clear the input buffer.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                // Adding an assignment - Collect details from the user:
                std::string subject, assignmentName;
                int deadline, duration, size, groupSize;
                double weight;
                bool groupWork;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter the assignment details:\n";
                std::cout << "Subject: ";
                std::getline(std::cin, subject);
                std::cout << "Name: ";
                std::getline(std::cin, assignmentName);
                std::cout << "Deadline (in days): ";
                std::cin >> deadline;
                std::cout << "Duration (in hours): ";
                std::cin >> duration;
                std::cout << "Weight (as a percentage): ";
                std::cin >> weight;
                std::cout << "Size (Big/Medium/Small - 1/2/3): ";
                std::cin >> size;
                std::cout << "Group Work (1 for Yes, 0 for No): ";
                std::cin >> groupWork;

                if (groupWork) {
                    std::cout << "Group Size: ";
                    std::cin >> groupSize;
                } else {
                    groupSize = 1;
                }

                Database::addAssignment(dbName, subject, assignmentName, deadline, duration, weight, size, groupWork, groupSize);

                std::cout << "Assignment added successfully.\n";
                break;
            }
            case 2: {
                // Deleting an assignment by its ID - Collect ID from the user:
                std::cout << "Enter the ID of the assignment to delete: ";
                int id;
                std::cin >> id;

                Database::deleteAssignment(dbName, id);
                std::cout << "Assignment deleted successfully.\n";
                break;
            }
            case 3: {
                // Run priority-based scheduler - Collect study hours from the user (weekday & weekend):
                int weekdayHours, weekendHours;
                std::cout << "Enter weekday study hours: ";
                std::cin >> weekdayHours;
                std::cout << "Enter weekend study hours: ";
                std::cin >> weekendHours;

                Planner::scheduler({}, weekdayHours, weekendHours, name);
                std::cout << "Schedule saved to Data/" << name << "_schedule.ics\n";
                break;
            }
            case 4: {
                // Display options menu:
                DisplayFunctions::displayMenu({});
                break;
            }
            case 5: {
                // Exit program:
                std::cout << "Goodbye!\n";
                return 0;
            }
        }
    }

    return 0;
}