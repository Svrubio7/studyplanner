#include "../include/displayfunctions.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

// Display all assignments
void DisplayFunctions::displayAllAssignments(const std::vector<AssignmentPtr>& assignments) {
    if (assignments.empty()) {
        std::cout << "No assignments to display.\n";
        return;
    }

    std::cout << "\nAll Assignments:\n";
    for (const auto& assignment : assignments) {
        assignment->display();
        std::cout << "---------------------------\n";
    }
}

// Display assignments filtered by subject
void DisplayFunctions::displayAssignmentsBySubject(const std::vector<AssignmentPtr>& assignments, const std::string& subject) {
    std::cout << "\nAssignments for Subject: " << subject << "\n";
    bool found = false;
    for (const auto& assignment : assignments) {
        if (assignment->getSubject() == subject) {
            assignment->display();
            std::cout << "---------------------------\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "No assignments found for subject: " << subject << "\n";
    }
}

// Display assignments sorted by shortest deadline
void DisplayFunctions::displayAssignmentsByShortestDeadline(const std::vector<AssignmentPtr>& assignments) {
    if (assignments.empty()) {
        std::cout << "No assignments to display.\n";
        return;
    }

    std::cout << "\nAssignments by Shortest Deadline:\n";
    std::vector<AssignmentPtr> sortedAssignments(assignments);

    std::sort(sortedAssignments.begin(), sortedAssignments.end(),
              [](const AssignmentPtr& a, const AssignmentPtr& b) {
                  return a->getDeadline() < b->getDeadline();
              });

    for (const auto& assignment : sortedAssignments) {
        assignment->display();
        std::cout << "---------------------------\n";
    }
}

// Display assignments sorted by biggest duration
void DisplayFunctions::displayAssignmentsByBiggestDuration(const std::vector<AssignmentPtr>& assignments) {
    if (assignments.empty()) {
        std::cout << "No assignments to display.\n";
        return;
    }

    std::cout << "\nAssignments by Biggest Duration:\n";
    std::vector<AssignmentPtr> sortedAssignments(assignments);

    std::sort(sortedAssignments.begin(), sortedAssignments.end(),
              [](const AssignmentPtr& a, const AssignmentPtr& b) {
                  return a->getDuration() > b->getDuration();
              });

    for (const auto& assignment : sortedAssignments) {
        assignment->display();
        std::cout << "---------------------------\n";
    }
}

// Display menu options for assignments
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

        // Input validation
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
