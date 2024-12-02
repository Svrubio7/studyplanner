#ifndef FILEEXCEPTION_HPP
#define FILEEXCEPTION_HPP

#include <exception>
#include <string>

class FileException : public std::exception {
private:
    std::string message;
public:
    explicit FileException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // FILEEXCEPTION_HPP