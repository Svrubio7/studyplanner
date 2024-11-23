#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <string>
#include <iostream>

class Assignment {
private:
    std::string subject;
    std::string name;
    int deadline; // Remaining days to complete the assignment
    int duration; // Total required hours to complete the assignment
    float weight; // Importance of the assignment
    int size; // Size of the assignment or team (if group work)
    bool groupWork; // Indicates if the assignment is group work
    int groupSize; // Number of members in the group
    int realDuration; // Duration adjusted for group size
    int priority; // Priority score for scheduling

public:
    // Default constructor
    Assignment();

    // Parameterized constructor
    Assignment(const std::string& subject, const std::string& name, int deadline, int duration,
               float weight, int size, bool groupWork, int groupSize);

    // Copy constructor
    Assignment(const Assignment& other);

    // Move constructor
    Assignment(Assignment&& other) noexcept;

    // Copy assignment operator
    Assignment& operator=(const Assignment& other);

    // Move assignment operator
    Assignment& operator=(Assignment&& other) noexcept;

    // Destructor
    ~Assignment();

    // Setters and Getters for Priority
    void setPriority(int priority);
    int getPriority() const;

    // State modification methods
    void decreaseDuration(int hours); // Decrease remaining duration
    void decreaseDeadline(int days); // Decrease remaining deadline

    // Getters for private members
    const std::string& getSubject() const;
    const std::string& getName() const;
    int getDeadline() const;
    int getDuration() const;
    float getWeight() const;
    int getSize() const;
    bool isGroupWork() const;
    int getGroupSize() const;
    int getRealDuration() const;

    // Display function
    void display() const;
};

#endif // ASSIGNMENT_HPP
