#include "Assignments_DB.hpp"
#include <sqlite3.h>
#include <iostream>

// Initialize the SQLite database:
void Database::initialize(const std::string& dbName) {
    sqlite3* db;
    if (sqlite3_open(dbName.c_str(), &db) == SQLITE_OK) {
        std::cout << "Database initialized: " << dbName << std::endl;
    } else {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}

// Create the Assignments table in the database:
void Database::createAssignmentTable(const std::string& dbName) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS Assignments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            subject TEXT,
            name TEXT,
            deadline INTEGER,
            duration INTEGER,
            weight REAL,
            size INTEGER,
            group_work BOOLEAN,
            group_size INTEGER
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully." << std::endl;
    }

    sqlite3_close(db);
}

// Add an assignment to the database:
void Database::addAssignment(
    const std::string& dbName, 
    const std::string& subject, 
    const std::string& name, 
    int deadline, 
    int duration, 
    double weight, 
    int size, 
    bool groupWork, 
    int groupSize
) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = R"(
        INSERT INTO Assignments (subject, name, deadline, duration, weight, size, group_work, group_size)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind the values to the statement: 
    sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, deadline);
    sqlite3_bind_int(stmt, 4, duration);
    sqlite3_bind_double(stmt, 5, weight);
    sqlite3_bind_int(stmt, 6, size);
    sqlite3_bind_int(stmt, 7, groupWork ? 1 : 0);
    sqlite3_bind_int(stmt, 8, groupSize);

    // Execute the statement: Insert the data into the table
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Data inserted successfully into Assignments table!" << std::endl;
    }

}


// Show all assignments currently in the database:
void Database::showAllAssignments(const std::string& dbName) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Assignments;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "Table: Assignments" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;
        std::cout << " ID | Subject | Name | Deadline | Duration | Weight | Size | Group Work | Group Size" << std::endl;
        std::cout << "-------------------------------------------------------------------------------------" << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(stmt, 0) << " | ";
            std::cout << sqlite3_column_text(stmt, 1) << " | ";
            std::cout << sqlite3_column_text(stmt, 2) << " | ";
            std::cout << sqlite3_column_int(stmt, 3) << " | ";
            std::cout << sqlite3_column_int(stmt, 4) << " | ";
            std::cout << sqlite3_column_double(stmt, 5) << " | ";
            std::cout << sqlite3_column_int(stmt, 6) << " | ";
            std::cout << (sqlite3_column_int(stmt, 7) ? "Yes" : "No") << " | ";
            std::cout << sqlite3_column_int(stmt, 8) << std::endl;
        }

        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Error querying table: " << sqlite3_errmsg(db) << std::endl;
    }



    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Delete an assignment from the database:
void Database::deleteAssignment(const std::string& dbName, int assignmentId) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);
    std::string sql = "DELETE FROM Assignments WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, assignmentId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error deleting data: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Assignment deleted successfully from database.\n";
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Update the duration of an assignment in the database:
void Database::updateAssignmentDuration(const std::string& dbName, int id, int newDuration) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "UPDATE Assignments SET duration = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, newDuration); // Bind the new duration
        sqlite3_bind_int(stmt, 2, id);          // Bind the ID

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating assignment duration: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Successfully updated assignment duration for ID: " << id << std::endl;
        }
    } else {
        std::cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}