#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>

ResourceManager::ResourceManager(const std::string& filename) {
    dataSourceFile = filename;
}

bool ResourceManager::loadResources() {
    std::ifstream inFile(dataSourceFile);
    if (!inFile.is_open()) {
        std::cout << "Error: Could not open resource file at " << dataSourceFile << "\n";
        return false;
    }

    resources.clear();
    std::string line;
    
    while (std::getline(inFile, line)) {
        if (line != "") { 
            Resource newResource = Resource::fromFileString(line);
            resources.push_back(newResource);
        }
    }
    
    inFile.close();
    return true;
}

bool ResourceManager::saveResources() const {
    std::ofstream outFile(dataSourceFile);
    if (!outFile.is_open()) {
        std::cout << "Error: Could not open resource file for writing at " << dataSourceFile << "\n";
        return false;
    }

    for (size_t i = 0; i < resources.size(); i++) {
        outFile << resources[i].toFileString() << "\n";
    }

    outFile.close();
    return true;
}

void ResourceManager::displayAllResources() const {
    if (resources.size() == 0) {
        std::cout << "No resources available in the system.\n";
        return;
    }

    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(28) << "Name"
              << std::setw(20) << "Type"
              << std::setw(14) << "Status"
              << std::setw(8)  << "Usage"
              << std::endl;
    std::cout << "--------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < resources.size(); i++) {
        resources[i].display();
    }
}

void ResourceManager::displayAvailableResources() const {
    bool found = false;
    
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(28) << "Name"
              << std::setw(20) << "Type"
              << std::setw(14) << "Status"
              << std::setw(8)  << "Usage"
              << std::endl;
    std::cout << "--------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < resources.size(); i++) {
        if (resources[i].isAvailable()) {
            resources[i].display();
            found = true;
        }
    }

    if (found == false) {
        std::cout << "No resources are currently available.\n";
    }
}

Resource* ResourceManager::getResourceById(const std::string& id) {
    for (size_t i = 0; i < resources.size(); i++) {
        if (resources[i].getId() == id) {
            return &resources[i]; 
        }
    }
    return nullptr;
}

bool ResourceManager::resourceExists(const std::string& id) const {
    for (size_t i = 0; i < resources.size(); i++) {
        if (resources[i].getId() == id) {
            return true;
        }
    }
    return false;
}
