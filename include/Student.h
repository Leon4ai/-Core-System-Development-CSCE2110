#ifndef STUDENT_H
#define STUDENT_H

#include <string>

// Student: id + name. Reservations already store these fields;
// this class exists so the design matches the assignment list.
class Student {
private:
    int studentId;
    std::string name;

public:
    Student();
    Student(int id, const std::string& name);

    int getId() const;
    std::string getName() const;
    void setId(int id);
    void setName(const std::string& name);
};

#endif
