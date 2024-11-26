#include <iostream>
#include <limits>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <filesystem>
#include "../include/assignment.hpp"
#include "../include/planner.hpp"
#include "../include/displayfunctions.hpp"

// Check if user data file exists
bool userExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.is_open();
}

// Ensure the Data directory exists
void ensureDataDirectoryExists() {
    if (!std::filesystem::exists("Data")) {
        if (!std::filesystem::create_directory("Data")) {
            std::cerr << "Error: Could not create 'Data' directory.\n";
            exit(1); // Exit if unable to create the directory
        }
    }
}

int main() {
    // Ensure the Data directory exists
    ensureDataDirectoryExists();

    // Greeting and user setup
    std::cout << "Enter your name: ";
    std::string name;
    std::cin >> name;

    // Define the user file path
    std::string userFile = "Data/" + name + ".json";

    // Check if the user file exists
    if (!userExists(userFile)) {
        // Create the file directly
        std::ofstream MyFile(userFile);
        if (!MyFile) {
            std::cerr << "Error: Could not create the file for the user at: " << userFile << "\n";
            return 1;
        }
        MyFile << "[]"; // Initialize the file with an empty JSON array
        MyFile.close();
        std::cout << "Welcome, " << name << "! A new file has been created for you.\n";
    } else {
        std::cout << "Welcome back, " << name << "!\n";
    }

    // Load assignments
    std::vector<Planner::AssignmentPtr> assignments = Planner::loadFromFile(userFile);

    // Main menu loop
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

        // Input validation
        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear(); // Clear the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                // Adding an assignment
                std::string subject, name;
                int deadline, duration, size, groupSize;
                float weight;
                bool groupWork;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Enter the assignment details:\n";
                std::cout << "Subject: ";
                std::getline(std::cin, subject);
                std::cout << "Name: ";
                std::getline(std::cin, name);
                std::cout << "Deadline (in days): ";
                std::cin >> deadline;
                std::cout << "Duration (in hours): ";
                std::cin >> duration;
                std::cout << "Weight (as a percentage): ";
                std::cin >> weight;
                std::cout << "Size Big/Medium/Small (1/2/3): ";
                std::cin >> size;
                std::cout << "Group Work (1 for Yes, 0 for No): ";
                std::cin >> groupWork;

                if (groupWork) {
                    std::cout << "Group Size: ";
                    std::cin >> groupSize;
                } else {
                    groupSize = 1;
                }

                auto newAssignment = std::make_shared<Assignment>(subject, name, deadline, duration, weight, size, groupWork, groupSize);
                assignments.push_back(newAssignment);

                // Save changes to the file
                Planner::saveToFile(userFile, assignments);

                std::cout << "Assignment added successfully.\n";
                break;
            }
            case 2: {
                // Deleting an assignment
                if (assignments.empty()) {
                    std::cout << "No assignments to delete.\n";
                    break;
                }

                std::cout << "Select the assignment to delete:\n";
                for (size_t i = 0; i < assignments.size(); ++i) {
                    std::cout << i + 1 << ". " << assignments[i]->getName() << "\n";
                }
                std::cout << "Enter your choice: ";

                size_t deleteIndex;
                std::cin >> deleteIndex;

                if (deleteIndex > 0 && deleteIndex <= assignments.size()) {
                    assignments.erase(assignments.begin() + (deleteIndex - 1));

                    // Save changes to the file
                    Planner::saveToFile(userFile, assignments);

                    std::cout << "Assignment deleted successfully.\n";
                } else {
                    std::cout << "Invalid choice.\n";
                }
                break;
            }
            case 3: {
                // Run priority-based scheduler
                if (assignments.empty()) {
                    std::cout << "No assignments to schedule.\n";
                    break;
                }

                int weekdayHours, weekendHours;
                std::cout << "Enter weekday study hours: ";
                std::cin >> weekdayHours;
                std::cout << "Enter weekend study hours: ";
                std::cin >> weekendHours;

                // Pass the user name to the scheduler
                Planner::scheduler(assignments, weekdayHours, weekendHours, name);
                std::cout << "Schedule saved to Data/" << name << "_schedule.ics\n";
                break;
            }
            case 4: {
                // Display options menu
                DisplayFunctions::displayMenu(assignments);
                break;
            }
            case 5: {
                // Exit program
                std::cout << "Goodbye!\n";

                // Save changes before exiting
                Planner::saveToFile(userFile, assignments);
                return 0;
            }
        }
    }

    return 0;
}
