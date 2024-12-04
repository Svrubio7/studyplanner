#include "../include/planner.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <ctime>
#include <sstream>

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

void Planner::addToICSFile(const std::string& icsFilePath, const std::string& assignmentName, int dayOffset, int hour) {
    std::ofstream icsFile(icsFilePath, std::ios::app);

    if (!icsFile.is_open()) {
        std::cerr << "Error: Could not open ICS file for writing.\n";
        return;
    }

    // Get the current date
    std::time_t now = std::time(nullptr);
    std::tm* timeInfo = std::localtime(&now);

    // Add the day offset and set the event start and end times
    timeInfo->tm_mday += dayOffset;
    timeInfo->tm_hour = 18 + hour;  // Start time: 6 PM + scheduled hour
    timeInfo->tm_min = 0;
    timeInfo->tm_sec = 0;
    std::time_t startTime = std::mktime(timeInfo);

    timeInfo->tm_hour += 1;  // End time: 1 hour after start
    std::time_t endTime = std::mktime(timeInfo);

    // Convert start and end times to the required format
    char startBuffer[16], endBuffer[16];
    std::strftime(startBuffer, sizeof(startBuffer), "%Y%m%dT%H%M%S", std::localtime(&startTime));
    std::strftime(endBuffer, sizeof(endBuffer), "%Y%m%dT%H%M%S", std::localtime(&endTime));

    // Write the event details to the ICS file
    icsFile << "BEGIN:VEVENT\n";
    icsFile << "SUMMARY:" << assignmentName << "\n";
    icsFile << "DTSTART:" << startBuffer << "\n";
    icsFile << "DTEND:" << endBuffer << "\n";
    icsFile << "DESCRIPTION:Scheduled Assignment\n";
    icsFile << "STATUS:CONFIRMED\n";
    icsFile << "END:VEVENT\n";

    icsFile.close();
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
void Planner::scheduler(const std::vector<AssignmentPtr>& assignments, int weekdayStudyHours, int weekendStudyHours, const std::string& userName) {
    // Define the ICS file path based on the user name
    std::string icsFilePath = "Data/" + userName + "_schedule.ics";

    // Ensure the ICS file starts with a proper calendar header
    std::ofstream icsFile(icsFilePath, std::ios::trunc);
    if (icsFile.is_open()) {
        icsFile << "BEGIN:VCALENDAR\n";
        icsFile << "VERSION:2.0\n";
        icsFile << "PRODID:-//Planner App//EN\n";
        icsFile.close();
    } else {
        std::cerr << "Error: Could not create ICS file.\n";
        return;
    }

    std::vector<AssignmentPtr> assignmentList(assignments);
    int day = 1;

    while (!assignmentList.empty()) {
        std::cout << "\nDay " << day << ":\n";
        int studyHours = (day % 6 == 0 || day % 7 == 0) ? weekendStudyHours : weekdayStudyHours;
        auto compare = [](const AssignmentPtr& a, const AssignmentPtr& b) {
            return a->getPriority() < b->getPriority();
        };
        std::priority_queue<AssignmentPtr, std::vector<AssignmentPtr>, decltype(compare)> priorityQueue(compare);

        for (const auto& assignment : assignmentList) {
            int priority = calculatePriority(*assignment, studyHours);
            assignment->setPriority(priority);
            priorityQueue.push(assignment);
        }

        for (int i = 0; i < studyHours; ++i) {
            if (priorityQueue.empty())
                break;

            auto currentAssignment = priorityQueue.top();
            priorityQueue.pop();

            std::cout << "Hour " << (i + 1) << ": " << currentAssignment->getName() << "\n";
            currentAssignment->decreaseDuration(1);

            // Add the scheduled assignment to the ICS file
            addToICSFile(icsFilePath, currentAssignment->getName(), day, i);

            if (currentAssignment->getRealDuration() <= 0) {
                auto it = std::find(assignmentList.begin(), assignmentList.end(), currentAssignment);
                if (it != assignmentList.end())
                    assignmentList.erase(it);
            } else {
                currentAssignment->setPriority(calculatePriority(*currentAssignment, studyHours));
                priorityQueue.push(currentAssignment);
            }
        }

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

    // Add the ICS footer
    icsFile.open(icsFilePath, std::ios::app);
    if (icsFile.is_open()) {
        icsFile << "END:VCALENDAR\n";
        icsFile.close();
    }
}

