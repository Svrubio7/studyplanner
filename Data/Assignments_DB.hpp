#ifndef ASSIGNMENTS_DB_HPP
#define ASSIGNMENTS_DB_HPP

#include <string>

class Database {
public:
    static void initialize(const std::string& dbName);
    static void createAssignmentTable(const std::string& dbName);
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
    static void showAllAssignments(const std::string& dbName);
};

#endif // ASSIGNMENTS_DB_HPP
