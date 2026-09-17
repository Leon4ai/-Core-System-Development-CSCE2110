#ifndef WAITREQUEST_H
#define WAITREQUEST_H

#include <string>

class WaitRequest {
private:
    std::string resourceId;
    int studentId;
    std::string studentName;
    std::string date;
public:
    WaitRequest();
    WaitRequest(const std::string& resId, int stuId, const std::string& stuName, const std::string& d);

    // getter
    std::string getResourceId() const;
    int getStudentId() const;
    std::string getStudentName() const;
    std::string getDate() const;

    //helpers
    std::string toFileString() const;
    static WaitRequest fromFileString(const std::string& line);
};

#endif
