#include "../include/planner.hpp"
#include "Assignments_DB.hpp" 
#include <iostream>
#include <fstream>
#include <queue>
#include <sqlite3.h>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <ctime>
#include <sstream>

// Fetch assignments from database:
std::vector<Planner::AssignmentPtr> Planner::loadFromDatabase(const std::string& dbName) {
    std::vector<AssignmentPtr> assignments;

    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Assignments;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // Iterate through the result set and populate the assignments vector:
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            auto assignment = std::make_shared<Assignment>();
            assignment->fromSQLiteRow(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                sqlite3_column_double(stmt, 5),
                sqlite3_column_int(stmt, 6),
                sqlite3_column_int(stmt, 7),
                sqlite3_column_int(stmt, 8)
            );
            assignments.push_back(assignment);
        }
    } else {
        std::cerr << "Error loading assignments from database: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return assignments;
}

void Planner::addToICSFile(const std::string& icsFilePath, const std::string& assignmentName, int dayOffset, int hour) {
    // Open the ICS file for appending:
    std::ofstream icsFile(icsFilePath, std::ios::app);

    // Error handling for file opening:
    if (!icsFile.is_open()) {
        std::cerr << "Error: Could not open ICS file for writing.\n";
        return;
    }

    // Get the current date - used as the base for scheduling:
    std::time_t now = std::time(nullptr);
    std::tm* timeInfo = std::localtime(&now);

    // Add the day offset and set the event start and end times:
    timeInfo->tm_mday += dayOffset;
    timeInfo->tm_hour = 18 + hour;  // Start time: 6 PM + scheduled hour.
    timeInfo->tm_min = 0;
    timeInfo->tm_sec = 0;
    std::time_t startTime = std::mktime(timeInfo);

    timeInfo->tm_hour += 1;  // End time - 1 hour after start.
    std::time_t endTime = std::mktime(timeInfo);

    // Convert start and end times to the required format:
    char startBuffer[16], endBuffer[16];
    std::strftime(startBuffer, sizeof(startBuffer), "%Y%m%dT%H%M%S", std::localtime(&startTime));
    std::strftime(endBuffer, sizeof(endBuffer), "%Y%m%dT%H%M%S", std::localtime(&endTime));

    // Write the event details to the ICS file:
    icsFile << "BEGIN:VEVENT\n";
    icsFile << "SUMMARY:" << assignmentName << "\n";
    icsFile << "DTSTART:" << startBuffer << "\n";
    icsFile << "DTEND:" << endBuffer << "\n";
    icsFile << "DESCRIPTION:Scheduled Assignment\n";
    icsFile << "STATUS:CONFIRMED\n";
    icsFile << "END:VEVENT\n";

    icsFile.close();
}

int Planner::calculatePriority(const Assignment& assignment, int studyHoursPerDay) {
    // Priority calculation logic:
    int remainingHours = assignment.getDeadline() * studyHoursPerDay;
    int priority = 0;

    // Add priority based on deadline:
    if (assignment.getDeadline() < 2)
        priority += 10;
    else if (assignment.getDeadline() < 4)
        priority += 8;
    else if (assignment.getDeadline() < 6)
        priority += 6;
    else if (assignment.getDeadline() < 8)
        priority += 4;

    // Add priority based on remaining time:
    if ((remainingHours - assignment.getRealDuration()) < 2)
        priority += 20;
    else if ((remainingHours - assignment.getRealDuration()) < 4)
        priority += 15;
    else if ((remainingHours - assignment.getRealDuration()) < 6)
        priority += 10;

    // Add priority based on grade weight:
    if (assignment.getWeight() > 20)
        priority += 6;
    else if (assignment.getWeight() > 15)
        priority += 4;
    else if (assignment.getWeight() > 10)
        priority += 2;

    // Add priority based on size:
    if (assignment.getSize() == 1)
        priority += 3;
    else if (assignment.getSize() == 2)
        priority += 2;
    else if (assignment.getSize() == 3)
        priority += 1;

    return priority;
}

void Planner::scheduler(const std::vector<AssignmentPtr>&, int weekdayStudyHours, int weekendStudyHours, const std::string& userName) {
    // Define the database name:
    const std::string dbName = "assignments.db";

    // Load all assignments from the database:
    auto assignments = loadFromDatabase(dbName);

    // Define the ICS file path based on the user name:
    std::string icsFilePath = "Data/" + userName + "_schedule.ics";

    // Ensure the ICS file starts with a proper calendar header:
    std::ofstream icsFile(icsFilePath, std::ios::trunc);
    if (icsFile.is_open()) {
        icsFile << "BEGIN:VCALENDAR\nVERSION:2.0\nPRODID:-//Planner App//EN\n";
        icsFile.close();
    } else {
        std::cerr << "Error: Could not create ICS file.\n";
        return;
    }

    int day = 1;

    // Main scheduling loop:
    while (!assignments.empty()) {
        int studyHours = (day % 6 == 0 || day % 7 == 0) ? weekendStudyHours : weekdayStudyHours;

        // Priority queue for scheduling:
        auto compare = [](const AssignmentPtr& a, const AssignmentPtr& b) {
            return a->getPriority() < b->getPriority();
        };
        std::priority_queue<AssignmentPtr, std::vector<AssignmentPtr>, decltype(compare)> priorityQueue(compare);

        for (const auto& assignment : assignments) {
            assignment->setPriority(calculatePriority(*assignment, studyHours));
            priorityQueue.push(assignment);
        }

        // Allocate study hours to assignments:
        for (int i = 0; i < studyHours; ++i) {
            if (priorityQueue.empty()) break;

            auto currentAssignment = priorityQueue.top();
            priorityQueue.pop();

            std::cout << "Hour " << (i + 1) << ": " << currentAssignment->getName() << "\n";
            currentAssignment->decreaseDuration(1);

            // Add the scheduled assignment to the ICS file:
            addToICSFile(icsFilePath, currentAssignment->getName(), day, i);

            // Update the database with the new duration or delete completed assignments:
            if (currentAssignment->getRealDuration() <= 0) {
                Database::deleteAssignment(dbName, currentAssignment->getID());
                assignments = loadFromDatabase(dbName); // Reload assignments after deletion.
            } else {
                Database::updateAssignmentDuration(dbName, currentAssignment->getID(), currentAssignment->getRealDuration());
            }
        }

        // Decrease deadlines for all assignments:
        for (auto& assignment : assignments) {
            assignment->decreaseDeadline(1);
            if (assignment->getDeadline() <= 0) {
                std::cout << "Missed deadline for assignment: " << assignment->getName() << "\n";
                Database::deleteAssignment(dbName, assignment->getID());
            }
        }
        ++day;
    }

    // Add the ICS footer:
    icsFile.open(icsFilePath, std::ios::app);
    if (icsFile.is_open()) {
        icsFile << "END:VCALENDAR\n";
        icsFile.close();
    }
}
