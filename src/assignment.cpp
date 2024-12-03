#include "../include/assignment.hpp"
#include <iostream>
#include <tuple>

// Default constructor:
Assignment::Assignment()
    : id(0), subject("N/A"), name("N/A"), deadline(0), duration(1), weight(0.0),
      size(3), groupWork(false), groupSize(1), realDuration(1), priority(0) {}

// Parameterized constructor:
Assignment::Assignment(const std::string& subject, const std::string& name, int deadline,
                       int duration, double weight, int size, bool groupWork, int groupSize)
    : id(0), subject(subject), name(name), deadline(deadline), duration(duration), weight(weight),
      size(size), groupWork(groupWork), groupSize(groupSize), 
      realDuration(duration / (groupSize > 0 ? groupSize : 1)), priority(0) {}

// Copy constructor:
Assignment::Assignment(const Assignment& other)
    : id(other.id), subject(other.subject), name(other.name), deadline(other.deadline), 
      duration(other.duration), weight(other.weight), size(other.size), 
      groupWork(other.groupWork), groupSize(other.groupSize), 
      realDuration(other.realDuration), priority(other.priority) {
    std::cout << "Copy constructor called." << std::endl;
}

// Move constructor:
Assignment::Assignment(Assignment&& other) noexcept
    : id(other.id), subject(std::move(other.subject)), name(std::move(other.name)), 
      deadline(other.deadline), duration(other.duration), weight(other.weight), 
      size(other.size), groupWork(other.groupWork), groupSize(other.groupSize), 
      realDuration(other.realDuration), priority(other.priority) {
    std::cout << "Move constructor called." << std::endl;
}

// Copy assignment operator:
Assignment& Assignment::operator=(const Assignment& other) {
    if (this != &other) {
        id = other.id;
        subject = other.subject;
        name = other.name;
        deadline = other.deadline;
        duration = other.duration;
        weight = other.weight;
        size = other.size;
        groupWork = other.groupWork;
        groupSize = other.groupSize;
        realDuration = other.realDuration;
        priority = other.priority;
        std::cout << "Copy assignment operator called." << std::endl;
    }
    return *this;
}

// Move assignment operator:
Assignment& Assignment::operator=(Assignment&& other) noexcept {
    if (this != &other) {
        id = other.id;
        subject = std::move(other.subject);
        name = std::move(other.name);
        deadline = other.deadline;
        duration = other.duration;
        weight = other.weight;
        size = other.size;
        groupWork = other.groupWork;
        groupSize = other.groupSize;
        realDuration = other.realDuration;
        priority = other.priority;
        std::cout << "Move assignment operator called." << std::endl;
    }
    return *this;
}

// Destructor:
Assignment::~Assignment() {
    std::cout << "Destructor called for " << name << "." << std::endl;
}

// SQLite integration - Load from a row: get data from the database and assign it to the object
void Assignment::fromSQLiteRow(int id, const std::string& subject, const std::string& name,
                               int deadline, int duration, double weight, int size, int groupWork, int groupSize) {
    this->id = id;
    this->subject = subject;
    this->name = name;
    this->deadline = deadline;
    this->duration = duration;
    this->weight = weight;
    this->size = size;
    this->groupWork = (groupWork == 1);
    this->groupSize = groupSize;
    this->realDuration = duration / (groupSize > 0 ? groupSize : 1);
}

// SQLite integration - Convert to a row: get data from the object and assign it to a tuple for database insertion
std::tuple<std::string, std::string, int, int, double, int, int, int> Assignment::toSQLiteRow() const {
    return std::make_tuple(subject, name, deadline, duration, weight, size, groupWork ? 1 : 0, groupSize);
}

// Setters and Getters for Priority:
void Assignment::setPriority(int priority) { this->priority = priority; }
int Assignment::getPriority() const { return priority; }

// State modification methods:
void Assignment::decreaseDuration(int hours) { realDuration -= hours; }
void Assignment::decreaseDeadline(int days) { deadline -= days; }

// Getters for private members:
const std::string& Assignment::getSubject() const { return subject; }
const std::string& Assignment::getName() const { return name; }
int Assignment::getDeadline() const { return deadline; }
int Assignment::getDuration() const { return duration; }
double Assignment::getWeight() const { return weight; }
int Assignment::getSize() const { return size; }
bool Assignment::isGroupWork() const { return groupWork; }
int Assignment::getGroupSize() const { return groupSize; }
int Assignment::getRealDuration() const { return realDuration; }
int Assignment::getID() const { return id; }