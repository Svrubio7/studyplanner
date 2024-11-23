#ifndef PLANNER_HPP
#define PLANNER_HPP

#include <vector>
#include <string>
#include <memory>
#include "assignment.hpp"

class Planner {
public:
    using AssignmentPtr = std::shared_ptr<Assignment>;

    // Load assignments from a JSON file
    static std::vector<AssignmentPtr> loadFromFile(const std::string& filename);

    // Save assignments to a JSON file
    static void saveToFile(const std::string& filename, const std::vector<AssignmentPtr>& assignments);

    // Scheduler function to plan assignments
    static void scheduler(const std::vector<AssignmentPtr>& assignments, int weekdayStudyHours, int weekendStudyHours);

    // Helper function to calculate the priority of an assignment
    static int calculatePriority(const Assignment& assignment, int studyHoursPerDay);
};

#endif // PLANNER_HPP
