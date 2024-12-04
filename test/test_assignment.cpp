#include <gtest/gtest.h>
#include "assignment.hpp"

// Test the default constructor
TEST(AssignmentTest, DefaultConstructor) {
    Assignment assign;
    
    EXPECT_EQ(assign.getSubject(), "N/A");
    EXPECT_EQ(assign.getName(), "N/A");
    EXPECT_EQ(assign.getDeadline(), 0);
    EXPECT_EQ(assign.getDuration(), 1);
    EXPECT_FLOAT_EQ(assign.getWeight(), 0.0f);
    EXPECT_EQ(assign.getSize(), 3);
    EXPECT_FALSE(assign.isGroupWork());
    EXPECT_EQ(assign.getGroupSize(), 1);
    EXPECT_EQ(assign.getRealDuration(), 1);
    EXPECT_EQ(assign.getPriority(), 0);
}

// Test the parameterized constructor
TEST(AssignmentTest, ParameterizedConstructor) {
    Assignment assign("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    
    EXPECT_EQ(assign.getSubject(), "Math");
    EXPECT_EQ(assign.getName(), "Assignment 1");
    EXPECT_EQ(assign.getDeadline(), 7);
    EXPECT_EQ(assign.getDuration(), 5);
    EXPECT_FLOAT_EQ(assign.getWeight(), 10.0f);
    EXPECT_EQ(assign.getSize(), 4);
    EXPECT_TRUE(assign.isGroupWork());
    EXPECT_EQ(assign.getGroupSize(), 2);
    EXPECT_EQ(assign.getRealDuration(), 2);  // 5 / 2 = 2
}

// Test the decreaseDuration method
TEST(AssignmentTest, DecreaseDuration) {
    Assignment assign("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    
    assign.decreaseDuration(1);
    
    EXPECT_EQ(assign.getRealDuration(), 1);  // 2 - 1 = 1
}

// Test the decreaseDeadline method
TEST(AssignmentTest, DecreaseDeadline) {
    Assignment assign("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    
    assign.decreaseDeadline(2);
    
    EXPECT_EQ(assign.getDeadline(), 5);  // 7 - 2 = 5
}

// Test the copy constructor
TEST(AssignmentTest, CopyConstructor) {
    Assignment assign1("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    Assignment assign2 = assign1;

    EXPECT_EQ(assign2.getSubject(), "Math");
    EXPECT_EQ(assign2.getName(), "Assignment 1");
}

// Test the move constructor
TEST(AssignmentTest, MoveConstructor) {
    Assignment assign1("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    Assignment assign2 = std::move(assign1);
    
    EXPECT_EQ(assign2.getSubject(), "Math");
    EXPECT_EQ(assign2.getName(), "Assignment 1");
}

// Test the copy assignment operator
TEST(AssignmentTest, CopyAssignment) {
    Assignment assign1("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    Assignment assign2("Science", "Assignment 2", 10, 6, 15.0f, 3, false, 1);
    assign2 = assign1;

    EXPECT_EQ(assign2.getSubject(), "Math");
    EXPECT_EQ(assign2.getName(), "Assignment 1");
}

// Test the move assignment operator
TEST(AssignmentTest, MoveAssignment) {
    Assignment assign1("Math", "Assignment 1", 7, 5, 10.0f, 4, true, 2);
    Assignment assign2("Science", "Assignment 2", 10, 6, 15.0f, 3, false, 1);
    assign2 = std::move(assign1);

    EXPECT_EQ(assign2.getSubject(), "Math");
    EXPECT_EQ(assign2.getName(), "Assignment 1");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
