#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include <iostream>

class Resource {
private:
    std::string resourceId;
    std::string name;
    std::string type;
    bool available;
    int usageCount;

public:
    // Constructors
    Resource();
    Resource(const std::string& id, const std::string& name, const std::string& type, bool available = true, int usageCount = 0);

    // getters
    std::string getId() const;
    std::string getName() const;
    std::string getType() const;
    bool isAvailable() const;
    int getUsageCount() const;
    // Setters
    void setId(const std::string& id);
    void setName(const std::string& n);
    void setType(const std::string& t);
    void setAvailable(bool status);
    void incrementUsage();
    void setUsageCount(int count);

    // Utility & File I/O
    void display() const;
    std::string toFileString() const;
    
    // Static method for parsing CSV lines
    static Resource fromFileString(const std::string& line);
};

#endif
