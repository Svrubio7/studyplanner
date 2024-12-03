#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <tuple>

class Assignment {
private:
    int id; // Unique ID to each assignment for database integration
    std::string subject;
    std::string name;
    int deadline;     // Remaining days to complete the assignment
    int duration;     // Total required hours to complete the assignment
    double weight;    // Grade wight of the assignment (percentage)
    int size;         // Size of the assignment - (Big/Medium/Small) with int representation (1/2/3)
    bool groupWork;   // Indicates if the assignment is group work or not
    int groupSize;    // If GW - number of members in the group
    int realDuration; // Duration adjusted for group size
    int priority;     // Priority score for scheduling

public:
    // Default constructor:
    Assignment();

    // Parameterized constructor:
    Assignment(const std::string& subject, const std::string& name, int deadline, int duration,
               double weight, int size, bool groupWork, int groupSize);

    // Copy constructor:
    Assignment(const Assignment& other);

    // Move constructor:
    Assignment(Assignment&& other) noexcept;

    // Copy assignment operator:
    Assignment& operator=(const Assignment& other);

    // Move assignment operator:
    Assignment& operator=(Assignment&& other) noexcept;

    // Destructor:
    ~Assignment();

    // SQLite integration methods:
    void fromSQLiteRow(int id, const std::string& subject, const std::string& name, int deadline, 
                       int duration, double weight, int size, int groupWork, int groupSize);
    std::tuple<std::string, std::string, int, int, double, int, int, int> toSQLiteRow() const;

    // Setters and Getters for priority:
    void setPriority(int priority);
    int getPriority() const;

    // Getter for the assignment ID:
    int getID() const;

    // Table modification methods:
    void decreaseDuration(int hours); // Decrease remaining duration
    void decreaseDeadline(int days);  // Decrease remaining deadline

    // Getters for private members:
    const std::string& getSubject() const;
    const std::string& getName() const;
    int getDeadline() const;
    int getDuration() const;
    double getWeight() const;
    int getSize() const;
    bool isGroupWork() const;
    int getGroupSize() const;
    int getRealDuration() const;
};

#endif // ASSIGNMENT_HPP
