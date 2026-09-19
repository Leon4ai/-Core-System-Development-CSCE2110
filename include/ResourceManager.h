#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Resource.h"
#include <vector>

#include <string>

class ResourceManager {
private:
    std::vector<Resource> resources;
    std::string dataSourceFile;

public:
    ResourceManager(const std::string& filename = "data/resources.txt");

    // File oper
    bool loadResources();
    bool saveResources() const;

    //  Display Operations
    void displayAllResources() const;
    void displayAvailableResources() const;

    std::vector<Resource>& all() {
    return resources;}
const std::vector<Resource>& all() const {
    return resources;
}

    // helpers
    Resource* getResourceById(const std::string& id);
    bool resourceExists(const std::string& id) const;
};

#endif
