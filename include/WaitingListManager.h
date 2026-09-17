#ifndef WAITINGLISTMANAGER_H
#define WAITINGLISTMANAGER_H

#include <string>
#include <vector>
#include "WaitQueue.h"

#include "WaitRequest.h"

class WaitingListManager {
private:
    struct Entry {
        std::string resourceId;
        WaitQueue queue;
    };
    std::vector<Entry> lists;
    int indexOf(const std::string& resourceId) const;
public:
    WaitingListManager();

    void enqueue(const WaitRequest& request);
    bool dequeue(const std::string& resourceId, WaitRequest& out);
    bool peek(const std::string& resourceId, WaitRequest& out) const;
    
    bool isEmpty(const std::string& resourceId) const;
    int size(const std::string& resourceId) const;
    int totalWaiting() const;
    
    void displayAll() const;
    void displayForResource(const std::string& resourceId) const;
    
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif 
