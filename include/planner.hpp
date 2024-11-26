#ifndef PLANNER_HPP
#define PLANNER_HPP

#include "assignment.hpp"
#include <vector>
#include <string>
#include <memory>

// Namespace for organizing planner-related functionality
namespace Planner {
    // Define a shared pointer for assignments
    using AssignmentPtr = std::shared_ptr<Assignment>;

    // Function declarations

    // Load assignments from a file
    std::vector<AssignmentPtr> loadFromFile(const std::string& filename);

    // Save assignments to a file
    void saveToFile(const std::string& filename, const std::vector<AssignmentPtr>& assignments);

    // Calculate the priority of an assignment based on the given study hours
    int calculatePriority(const Assignment& assignment, int studyHoursPerDay);

    // Priority-based scheduler for assignments
    void scheduler(const std::vector<AssignmentPtr>& assignments, int weekdayStudyHours, int weekendStudyHours, const std::string& userName);

    // Add an assignment schedule to an ICS file
    void addToICSFile(const std::string& icsFilePath, const std::string& assignmentName, int dayOffset, int hour);
}

#endif // PLANNER_HPP
