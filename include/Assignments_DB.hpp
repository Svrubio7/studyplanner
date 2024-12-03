#ifndef ASSIGNMENTS_DB_HPP
#define ASSIGNMENTS_DB_HPP

#include <string>

// Database class to handle SQLite database operations:
class Database {
public:
    // Initialize the database:
    static void initialize(const std::string& dbName);
    // Create the assignments table in the database:
    static void createAssignmentTable(const std::string& dbName);
    // Add an assignment to the database:
    static void addAssignment(
        const std::string& dbName, 
        const std::string& subject, 
        const std::string& name, 
        int deadline, 
        int duration, 
        double weight, 
        int size, 
        bool groupWork, 
        int groupSize
    );
    // Show all assignments currently in the database:
    static void showAllAssignments(const std::string& dbName);
    // Delete an assignment from the database:
    static void deleteAssignment(const std::string& dbName, int id);
    // Update the duration of an assignment in the database:
    static void updateAssignmentDuration(const std::string& dbName, int id, int newDuration);

};

#endif // ASSIGNMENTS_DB_HPP
