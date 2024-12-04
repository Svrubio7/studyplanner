cmake_minimum_required(VERSION 3.10)
project(AssignmentScheduler)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for headers
include_directories(include)

# Source files for the main program
set(SRC_FILES
    src/assignment.cpp
    src/displayfunctions.cpp
    src/planner.cpp
)

# Test files
set(TEST_FILES
    test/test_assignment.cpp
    test/test_displayfunctions.cpp
    test/test_planner.cpp
)

# Main program file
set(MAIN_FILE src/main.cpp)

# Find GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Create the main program executable
add_executable(main_program ${SRC_FILES} ${MAIN_FILE})

# Create the test executable
add_executable(runTests ${SRC_FILES} ${TEST_FILES})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
