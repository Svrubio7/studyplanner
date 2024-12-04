#include "gtest/gtest.h"
#include "../include/planner.hpp"
#include "../include/assignment.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;

static std::shared_ptr<Assignment> createAssignment(const std::string& subject, const std::string& name, int deadline,
                                                    int duration, float weight, int size, bool groupWork, int groupSize) {
    return std::make_shared<Assignment>(subject, name, deadline, duration, weight, size, groupWork, groupSize);
}

// Test Planner::loadFromFile for valid input
TEST(PlannerTest, LoadFromFile_ValidFile) {
    // Create a temporary JSON file
    std::ofstream tempFile("temp.json");
    json testData = json::array({
        {{"subject", "Math"}, {"name", "Math Homework"}, {"deadline", 5}, {"duration", 10}, {"weight", 20.0}, {"size", 1}, {"group_work", false}, {"group_size", 1}},
        {{"subject", "Science"}, {"name", "Science Project"}, {"deadline", 7}, {"duration", 15}, {"weight", 25.0}, {"size", 2}, {"group_work", true}, {"group_size", 3}}
    });
    tempFile << testData.dump(4);
    tempFile.close();

    // Load assignments from file
    auto assignments = Planner::loadFromFile("temp.json");

    // Validate the results
    EXPECT_EQ(assignments.size(), 2);
    EXPECT_EQ(assignments[0]->getSubject(), "Math");
    EXPECT_EQ(assignments[0]->getName(), "Math Homework");
    EXPECT_EQ(assignments[1]->getSubject(), "Science");
    EXPECT_EQ(assignments[1]->getName(), "Science Project");

    // Clean up
    std::remove("temp.json");
}

// Test Planner::loadFromFile for invalid input
TEST(PlannerTest, LoadFromFile_InvalidFile) {
    auto assignments = Planner::loadFromFile("nonexistent.json");

    // Expect no assignments to be loaded
    EXPECT_TRUE(assignments.empty());
}

// Test Planner::saveToFile
TEST(PlannerTest, SaveToFile) {
    // Create assignments
    std::vector<Planner::AssignmentPtr> assignments = {
        createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1),
        createAssignment("Science", "Science Project", 7, 15, 25.0, 2, true, 3)
    };

    // Save assignments to file
    Planner::saveToFile("temp.json", assignments);

    // Read and validate the saved file
    std::ifstream file("temp.json");
    ASSERT_TRUE(file.is_open());

    json savedData;
    file >> savedData;

    EXPECT_EQ(savedData.size(), 2);
    EXPECT_EQ(savedData[0]["subject"], "Math");
    EXPECT_EQ(savedData[1]["name"], "Science Project");

    // Clean up
    std::remove("temp.json");
}

// Test Planner::addToICSFile
TEST(PlannerTest, AddToICSFile) {
    Planner::addToICSFile("test_schedule.ics", "Math Homework", 2, 6);

    // Read and validate the ICS file
    std::ifstream file("test_schedule.ics");
    ASSERT_TRUE(file.is_open());

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_NE(content.find("SUMMARY:Math Homework"), std::string::npos);
    EXPECT_NE(content.find("BEGIN:VEVENT"), std::string::npos);
    EXPECT_NE(content.find("END:VEVENT"), std::string::npos);

    // Clean up
    std::remove("test_schedule.ics");
}

// Test Planner::calculatePriority
TEST(PlannerTest, CalculatePriority) {
    auto assignment = createAssignment("Math", "Math Homework", 3, 10, 20.0, 1, false, 1);

    int priority = Planner::calculatePriority(*assignment, 5);

    // Validate the calculated priority
    EXPECT_GT(priority, 0); // Ensure priority is greater than 0
    EXPECT_LT(priority, 50); // Ensure priority is less than a reasonable max
}

TEST(PlannerTest, Scheduler_Simple) {
    // Create valid assignments
    std::vector<Planner::AssignmentPtr> assignments = {
        createAssignment("Math", "Math Homework", 2, 4, 20.0, 1, false, 1),
        createAssignment("Science", "Science Project", 3, 6, 25.0, 2, true, 3)
    };

    // Ensure Data directory exists
    #include <filesystem>
    if (!std::filesystem::exists("Data")) {
        std::filesystem::create_directory("Data");
    }

    // Run the scheduler
    Planner::scheduler(assignments, 3, 5, "test_user");

    // Validate the ICS file
    std::ifstream file("Data/test_user_schedule.ics");
    ASSERT_TRUE(file.is_open()) << "ICS file could not be created or opened.";

    // Validate content
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_NE(content.find("BEGIN:VCALENDAR"), std::string::npos);
    EXPECT_NE(content.find("END:VCALENDAR"), std::string::npos);
    EXPECT_NE(content.find("SUMMARY:Math Homework"), std::string::npos);
    EXPECT_NE(content.find("SUMMARY:Science Project"), std::string::npos);

    file.close();
    std::remove("Data/test_user_schedule.ics");
}
