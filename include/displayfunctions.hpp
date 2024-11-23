#ifndef DISPLAYFUNCTIONS_HPP
#define DISPLAYFUNCTIONS_HPP

#include <vector>
#include <memory>
#include <string>
#include "assignment.hpp"

class DisplayFunctions {
public:
    using AssignmentPtr = std::shared_ptr<Assignment>;

    // Display menu options for assignments
    static void displayMenu(const std::vector<AssignmentPtr>& assignments);

    // Display all assignments
    static void displayAllAssignments(const std::vector<AssignmentPtr>& assignments);

    // Display assignments filtered by subject
    static void displayAssignmentsBySubject(const std::vector<AssignmentPtr>& assignments, const std::string& subject);

    // Display assignments sorted by shortest deadline
    static void displayAssignmentsByShortestDeadline(const std::vector<AssignmentPtr>& assignments);

    // Display assignments sorted by biggest duration
    static void displayAssignmentsByBiggestDuration(const std::vector<AssignmentPtr>& assignments);
};

#endif // DISPLAYFUNCTIONS_HPP
