# studyplanner:
## Project Directory Structure:

```plaintext
project/
├── Data/
│   ├── <User_name>_schedule.ics      # Generated schedule file in ICS format
├── include/                          # Header files for the project
│   ├── assignment.hpp                # Assignment class definition
│   ├── planner.hpp                   # Planner namespace for scheduling logic
│   ├── displayfunctions.hpp          # Display-related functions
│   ├── Assignments_DB.hpp            # Database interaction utilities
│   ├── sqlite3.h                     # SQLite library header
├── src/                              # Source files for the project
│   ├── assignment.cpp                # Implementation of Assignment class
│   ├── planner.cpp                   # Implementation of scheduling logic
│   ├── displayfunctions.cpp          # Implementation of display functions
│   ├── Assignments_DB.cpp            # Implementation of database utilities
│   ├── main.cpp                      # Entry point of the application
