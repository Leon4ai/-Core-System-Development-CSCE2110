#include "Resource.h"
#include <sstream>
#include <iomanip>

// Empty resource used as a default / placeholder.
Resource::Resource()
    : resourceId(""), name(""), type(""), available(true), usageCount(0) {}

Resource::Resource(const std::string& id,
                   const std::string& name,
                   const std::string& type,
                   bool available,
                   int usageCount)
    : resourceId(id), name(name), type(type),
      available(available), usageCount(usageCount) {}

std::string Resource::getId() const { return resourceId; }
std::string Resource::getName() const { return name; }
std::string Resource::getType() const { return type; }
bool Resource::isAvailable() const { return available; }
int Resource::getUsageCount() const { return usageCount; }

void Resource::setId(const std::string& id) { resourceId = id; }
void Resource::setName(const std::string& n) { name = n; }
void Resource::setType(const std::string& t) { type = t; }
void Resource::setAvailable(bool status) { available = status; }
void Resource::incrementUsage() { ++usageCount; }   // called on each successful booking
void Resource::setUsageCount(int count) { usageCount = count; }

// Print one aligned table row: ID, name, type, status, usage.
void Resource::display() const {
    std::cout << std::left
              << std::setw(10) << resourceId
              << std::setw(28) << name
              << std::setw(20) << type
              << std::setw(14) << (available ? "Available" : "Reserved")
              << std::setw(8)  << usageCount
              << std::endl;
}

// Turn this object into the CSV line saved in resources.txt.
std::string Resource::toFileString() const {
    std::ostringstream oss;
    oss << resourceId << "," << name << "," << type << ","
        << (available ? 1 : 0) << "," << usageCount;
    return oss.str();
}

// Rebuild a Resource from one CSV line. Bad numbers become safe defaults.
Resource Resource::fromFileString(const std::string& line) {
    std::string id, name, type, availStr, usageStr;
    std::istringstream iss(line);

    std::getline(iss, id, ',');
    std::getline(iss, name, ',');
    std::getline(iss, type, ',');
    std::getline(iss, availStr, ',');
    std::getline(iss, usageStr, ',');

    bool avail = true;
    int usage = 0;
    if (!availStr.empty()) {
        avail = (availStr != "0" && availStr != "false" && availStr != "False");
    }
    if (!usageStr.empty()) {
        try { usage = std::stoi(usageStr); } catch (...) { usage = 0; }
    }
    return Resource(id, name, type, avail, usage);
}

