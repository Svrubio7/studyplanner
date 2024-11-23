#include "../include/assignment.hpp"

// Default constructor
Assignment::Assignment()
    : subject("N/A"), name("N/A"), deadline(0), duration(1), weight(0.0f),
      size(3), groupWork(false), groupSize(1), realDuration(1), priority(0) {}

// Parameterized constructor
Assignment::Assignment(const std::string& subject, const std::string& name, int deadline,
                       int duration, float weight, int size, bool groupWork, int groupSize)
    : subject(subject), name(name), deadline(deadline), duration(duration), weight(weight),
      size(size), groupWork(groupWork), groupSize(groupSize), realDuration(duration / groupSize), priority(0) {}

// Copy constructor
Assignment::Assignment(const Assignment& other)
    : subject(other.subject), name(other.name), deadline(other.deadline), duration(other.duration),
      weight(other.weight), size(other.size), groupWork(other.groupWork),
      groupSize(other.groupSize), realDuration(other.realDuration), priority(other.priority) {
    std::cout << "Copy constructor called." << std::endl;
}

// Move constructor
Assignment::Assignment(Assignment&& other) noexcept
    : subject(std::move(other.subject)), name(std::move(other.name)), deadline(other.deadline),
      duration(other.duration), weight(other.weight), size(other.size),
      groupWork(other.groupWork), groupSize(other.groupSize), realDuration(other.realDuration), priority(other.priority) {
    std::cout << "Move constructor called." << std::endl;
}

// Copy assignment operator
Assignment& Assignment::operator=(const Assignment& other) {
    if (this != &other) {
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

// Move assignment operator
Assignment& Assignment::operator=(Assignment&& other) noexcept {
    if (this != &other) {
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

// Destructor
Assignment::~Assignment() {
    std::cout << "Destructor called for " << name << "." << std::endl;
}

// Setters and Getters for Priority
void Assignment::setPriority(int priority) { this->priority = priority; }
int Assignment::getPriority() const { return priority; }

// State modification methods
void Assignment::decreaseDuration(int hours) { realDuration -= hours; }
void Assignment::decreaseDeadline(int days) { deadline -= days; }

// Getters for private members
const std::string& Assignment::getSubject() const { return subject; }
const std::string& Assignment::getName() const { return name; }
int Assignment::getDeadline() const { return deadline; }
int Assignment::getDuration() const { return duration; }
float Assignment::getWeight() const { return weight; }
int Assignment::getSize() const { return size; }
bool Assignment::isGroupWork() const { return groupWork; }
int Assignment::getGroupSize() const { return groupSize; }
int Assignment::getRealDuration() const { return realDuration; }

// Display function
void Assignment::display() const {
    std::cout << "Subject: " << subject << "\n"
              << "Name: " << name << "\n"
              << "Deadline: " << deadline << " days\n"
              << "Duration: " << duration << " hours\n"
              << "Weight: " << weight << "%\n"
              << "Size: " << size << "\n"
              << "Group Work: " << (groupWork ? "Yes" : "No") << "\n"
              << "Group Size: " << groupSize << "\n"
              << "Real Duration: " << realDuration << " hours\n"
              << "Priority: " << priority << "\n";
}
