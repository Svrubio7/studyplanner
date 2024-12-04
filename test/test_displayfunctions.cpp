#include "gtest/gtest.h"
#include "../include/displayfunctions.hpp"
#include "../include/assignment.hpp"
#include <vector>
#include <memory>
#include <sstream>

// Helper function to create shared_ptr<Assignment>
static std::shared_ptr<Assignment> createAssignment(const std::string& subject, const std::string& name, int deadline,
                                                    int duration, float weight, int size, bool groupWork, int groupSize) {
    return std::make_shared<Assignment>(subject, name, deadline, duration, weight, size, groupWork, groupSize);
}

// Test DisplayFunctions::displayAllAssignments with empty assignments list
TEST(DisplayFunctionsTest, DisplayAllAssignments_Empty) {
    std::vector<DisplayFunctions::AssignmentPtr> assignments;

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAllAssignments(assignments);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No assignments to display.\n");
}

// Test DisplayFunctions::displayAllAssignments with valid assignments
TEST(DisplayFunctionsTest, DisplayAllAssignments_NonEmpty) {
    auto assignment1 = createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1);
    auto assignment2 = createAssignment("Science", "Science Project", 7, 15, 25.0, 1, false, 1);

    std::vector<DisplayFunctions::AssignmentPtr> assignments = {assignment1, assignment2};

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAllAssignments(assignments);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedOutput =
        "\nAll Assignments:\n"
        "Subject: Math\n"
        "Name: Math Homework\n"
        "Deadline: 5 days\n"
        "Duration: 10 hours\n"
        "Weight: 20%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 10 hours\n"
        "Priority: 0\n"
        "---------------------------\n"
        "Subject: Science\n"
        "Name: Science Project\n"
        "Deadline: 7 days\n"
        "Duration: 15 hours\n"
        "Weight: 25%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 15 hours\n"
        "Priority: 0\n"
        "---------------------------\n";

    EXPECT_EQ(output, expectedOutput);
}

// Test DisplayFunctions::displayAssignmentsBySubject with no match
TEST(DisplayFunctionsTest, DisplayAssignmentsBySubject_NoMatch) {
    auto assignment = createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1);

    std::vector<DisplayFunctions::AssignmentPtr> assignments = {assignment};

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAssignmentsBySubject(assignments, "Science");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "\nAssignments for Subject: Science\nNo assignments found for subject: Science\n");
}

// Test DisplayFunctions::displayAssignmentsBySubject with a match
TEST(DisplayFunctionsTest, DisplayAssignmentsBySubject_MatchFound) {
    auto assignment1 = createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1);
    auto assignment2 = createAssignment("Science", "Science Project", 7, 15, 25.0, 1, false, 1);

    std::vector<DisplayFunctions::AssignmentPtr> assignments = {assignment1, assignment2};

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAssignmentsBySubject(assignments, "Math");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedOutput =
        "\nAssignments for Subject: Math\n"
        "Subject: Math\n"
        "Name: Math Homework\n"
        "Deadline: 5 days\n"
        "Duration: 10 hours\n"
        "Weight: 20%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 10 hours\n"
        "Priority: 0\n"
        "---------------------------\n";

    EXPECT_EQ(output, expectedOutput);
}

// Test DisplayFunctions::displayAssignmentsByShortestDeadline
TEST(DisplayFunctionsTest, DisplayAssignmentsByShortestDeadline) {
    auto assignment1 = createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1);
    auto assignment2 = createAssignment("Science", "Science Project", 3, 15, 25.0, 1, false, 1);

    std::vector<DisplayFunctions::AssignmentPtr> assignments = {assignment1, assignment2};

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAssignmentsByShortestDeadline(assignments);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedOutput =
        "\nAssignments by Shortest Deadline:\n"
        "Subject: Science\n"
        "Name: Science Project\n"
        "Deadline: 3 days\n"
        "Duration: 15 hours\n"
        "Weight: 25%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 15 hours\n"
        "Priority: 0\n"
        "---------------------------\n"
        "Subject: Math\n"
        "Name: Math Homework\n"
        "Deadline: 5 days\n"
        "Duration: 10 hours\n"
        "Weight: 20%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 10 hours\n"
        "Priority: 0\n"
        "---------------------------\n";

    EXPECT_EQ(output, expectedOutput);
}

// Test DisplayFunctions::displayAssignmentsByBiggestDuration
TEST(DisplayFunctionsTest, DisplayAssignmentsByBiggestDuration) {
    auto assignment1 = createAssignment("Math", "Math Homework", 5, 10, 20.0, 1, false, 1);
    auto assignment2 = createAssignment("Science", "Science Project", 7, 15, 25.0, 1, false, 1);

    std::vector<DisplayFunctions::AssignmentPtr> assignments = {assignment1, assignment2};

    testing::internal::CaptureStdout();
    DisplayFunctions::displayAssignmentsByBiggestDuration(assignments);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedOutput =
        "\nAssignments by Biggest Duration:\n"
        "Subject: Science\n"
        "Name: Science Project\n"
        "Deadline: 7 days\n"
        "Duration: 15 hours\n"
        "Weight: 25%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 15 hours\n"
        "Priority: 0\n"
        "---------------------------\n"
        "Subject: Math\n"
        "Name: Math Homework\n"
        "Deadline: 5 days\n"
        "Duration: 10 hours\n"
        "Weight: 20%\n"
        "Size: 1\n"
        "Group Work: No\n"
        "Group Size: 1\n"
        "Real Duration: 10 hours\n"
        "Priority: 0\n"
        "---------------------------\n";

    EXPECT_EQ(output, expectedOutput);
}
