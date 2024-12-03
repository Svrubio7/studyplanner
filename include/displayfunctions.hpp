#ifndef DISPLAYFUNCTIONS_HPP
#define DISPLAYFUNCTIONS_HPP

#include <string>
#include <vector>
#include <memory>
#include "assignment.hpp"

// Class to display menu options and assignments:
class DisplayFunctions {
public:
    // Typedef for shared pointer to Assignment:
    using AssignmentPtr = std::shared_ptr<Assignment>;

    // Display menu options for assignments:
    static void displayMenu(const std::vector<AssignmentPtr>& assignments);

    // Display all assignments using database:
    static void displayAllAssignments(const std::vector<AssignmentPtr>& assignments = {});

    // Display assignments filtered by subject:
    static void displayAssignmentsBySubject(const std::vector<AssignmentPtr>& assignments = {}, const std::string& subject = "");

    // Display assignments sorted by shortest deadline:
    static void displayAssignmentsByShortestDeadline(const std::vector<AssignmentPtr>& assignments = {});

    // Display assignments sorted by biggest duration:
    static void displayAssignmentsByBiggestDuration(const std::vector<AssignmentPtr>& assignments = {});
};

#endif // DISPLAYFUNCTIONS_HPP
