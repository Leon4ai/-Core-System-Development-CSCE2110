//old version.

/*S#include "Student.h"

Student::Student() : studentId(0), name("") {}

Student::Student(int id, const std::string& name)
    : studentId(id), name(name) {}

int Student::getId() const { return studentId; }
std::string Student::getName() const { return name; }
void Student::setId(int id) { studentId = id; }
void Student::setName(const std::string& n) { name = n; }
tudent.cpp
*/

//updated version.

#include "Student.h"

Student::Student()
    : studentId(0), name("") {
}

Student::Student(
    int id,
    const std::string& studentName
)
    : studentId(id), name(studentName) {
}

int Student::getId() const {
    return studentId;
}

std::string Student::getName() const {
    return name;
}

void Student::setId(int id) {
    studentId = id;
}

void Student::setName(const std::string& studentName) {
    name = studentName;
}
