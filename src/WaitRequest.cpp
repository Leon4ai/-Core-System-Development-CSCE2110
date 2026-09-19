#include "WaitRequest.h"

#include <sstream>

WaitRequest::WaitRequest()
    : studentId(0) {
}

WaitRequest::WaitRequest(
    const std::string& resource,
    int student,
    const std::string& name,
    const std::string& requestedDate
)
    : resourceId(resource),
      studentId(student),
      studentName(name),
      date(requestedDate) {
}

std::string WaitRequest::getResourceId() const {
    return resourceId;
}

int WaitRequest::getStudentId() const {
    return studentId;
}

std::string WaitRequest::getStudentName() const {
    return studentName;
}

std::string WaitRequest::getDate() const {
    return date;
}

std::string WaitRequest::toFileString() const {
    return resourceId + "," +
           std::to_string(studentId) + "," +
           studentName + "," +
           date;
}

WaitRequest WaitRequest::fromFileString(
    const std::string& line
) {
    std::istringstream input(line);

    std::string resource;
    std::string studentText;
    std::string name;
    std::string requestedDate;

    std::getline(input, resource, ',');
    std::getline(input, studentText, ',');
    std::getline(input, name, ',');
    std::getline(input, requestedDate);

    int student = 0;

    try {
        std::size_t charactersUsed;
        student = std::stoi(studentText, &charactersUsed);

        if (charactersUsed != studentText.size()) {
            student = 0;
        }
    } catch (...) {
        student = 0;
    }

    return WaitRequest(
        resource,
        student,
        name,
        requestedDate
    );
}
