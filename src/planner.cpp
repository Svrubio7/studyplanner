#include "../include/planner.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iterator>

// Use the nlohmann JSON namespace
using json = nlohmann::json;

// Implementation of loadFromFile
std::vector<Planner::AssignmentPtr> Planner::loadFromFile(const std::string& filename) {
    std::vector<AssignmentPtr> assignments;

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading.\n";
        return assignments; // Return an empty vector
    }

    try {
        // Parse the JSON file
        json jsonData;
        file >> jsonData;

        // Convert JSON objects to Assignment instances
        for (const auto& obj : jsonData) {
            auto assignment = std::make_shared<Assignment>(
                obj.at("subject").get<std::string>(),
                obj.at("name").get<std::string>(),
                obj.at("deadline").get<int>(),
                obj.at("duration").get<int>(),
                obj.at("weight").get<float>(),
                obj.at("size").get<int>(),
                obj.at("group_work").get<bool>(),
                obj.at("group_size").get<int>()
            );

            assignments.push_back(assignment);
        }
    } catch (const json::exception& e) {
        std::cerr << "Error: Failed to parse JSON - " << e.what() << "\n";
    }

    return assignments;
}

void Planner::saveToFile(const std::string& filename, const std::vector<AssignmentPtr>& assignments) {
    std::ofstream file(filename, std::ios::trunc); // Open file in truncate mode to overwrite existing data
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    nlohmann::json jsonData;

    // Serialize each assignment into JSON format
    for (const auto& assignment : assignments) {
        jsonData.push_back({
            {"subject", assignment->getSubject()},
            {"name", assignment->getName()},
            {"deadline", assignment->getDeadline()},
            {"duration", assignment->getDuration()},
            {"weight", assignment->getWeight()},
            {"size", assignment->getSize()},
            {"group_work", assignment->isGroupWork()},
            {"group_size", assignment->getGroupSize()}
        });
    }

    // Write JSON data to file
    file << jsonData.dump(4); // Pretty print with 4-space indentation
    file.close();
}


// Helper function to calculate priority
int Planner::calculatePriority(const Assignment& assignment, int studyHoursPerDay) {
    int remainingHours = assignment.getDeadline() * studyHoursPerDay;
    int priority = 0;

    // Add priority based on deadline
    if (assignment.getDeadline() < 2)
        priority += 10;
    else if (assignment.getDeadline() < 4)
        priority += 8;
    else if (assignment.getDeadline() < 6)
        priority += 6;
    else if (assignment.getDeadline() < 8)
        priority += 4;

    // Add priority based on remaining time
    if ((remainingHours - assignment.getRealDuration()) < 2)
        priority += 20;
    else if ((remainingHours - assignment.getRealDuration()) < 4)
        priority += 15;
    else if ((remainingHours - assignment.getRealDuration()) < 6)
        priority += 10;

    // Add priority based on weight
    if (assignment.getWeight() > 20)
        priority += 6;
    else if (assignment.getWeight() > 15)
        priority += 4;
    else if (assignment.getWeight() > 10)
        priority += 2;

    // Add priority based on size
    if (assignment.getSize() == 1)
        priority += 3;
    else if (assignment.getSize() == 2)
        priority += 2;
    else if (assignment.getSize() == 3)
        priority += 1;

    return priority;
}

// Scheduler implementation using a priority queue
void Planner::scheduler(const std::vector<AssignmentPtr>& assignments, int weekdayStudyHours, int weekendStudyHours) {
    // Copy assignments to work with a modifiable list
    std::vector<AssignmentPtr> assignmentList(assignments);
    int day = 1; // Track the day

    while (!assignmentList.empty()) {
        std::cout << "Day " << day << ":\n";

        // Determine study hours for the day
        int studyHours = (day % 6 == 0 || day % 7 == 0) ? weekendStudyHours : weekdayStudyHours;

        // Priority queue to sort assignments by priority (max-heap)
        auto compare = [](const AssignmentPtr& a, const AssignmentPtr& b) {
            return a->getPriority() < b->getPriority(); // Higher priority first
        };
        std::priority_queue<AssignmentPtr, std::vector<AssignmentPtr>, decltype(compare)> priorityQueue(compare);

        // Calculate priorities and populate the queue
        for (const auto& assignment : assignmentList) {
            int priority = calculatePriority(*assignment, studyHours);
            assignment->setPriority(priority); // Update the priority
            priorityQueue.push(assignment);
        }

        // Use all study hours in the day
        for (int i = 0; i < studyHours; ++i) {
            if (priorityQueue.empty())
                break;

            // Get the highest-priority assignment
            auto currentAssignment = priorityQueue.top();
            priorityQueue.pop();

            std::cout << "Hour " << (i + 1) << ": " << currentAssignment->getName() << "\n";

            // Work on the assignment
            currentAssignment->decreaseDuration(1);

            // If the assignment is completed, remove it from the list
            if (currentAssignment->getRealDuration() <= 0) {
                auto it = std::find(assignmentList.begin(), assignmentList.end(), currentAssignment);
                if (it != assignmentList.end())
                    assignmentList.erase(it);
            } else {
                // Recalculate and reinsert into the queue
                currentAssignment->setPriority(calculatePriority(*currentAssignment, studyHours));
                priorityQueue.push(currentAssignment);
            }
        }

        // Reduce the deadline for all assignments
        for (auto it = assignmentList.begin(); it != assignmentList.end();) {
            (*it)->decreaseDeadline(1);
            if ((*it)->getDeadline() <= 0) {
                std::cout << "Missed deadline for assignment: " << (*it)->getName() << "\n";
                it = assignmentList.erase(it);
            } else {
                ++it;
            }
        }

        ++day;
    }
}
