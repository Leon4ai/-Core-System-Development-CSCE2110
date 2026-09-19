#include "WaitingListManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>

WaitingListManager::WaitingListManager() {
}
// Linear lookup of which Entry holds this resource's queue.
int WaitingListManager::indexOf(const std::string& resourceId) const {
    for (std::size_t i = 0; i < lists.size(); ++i) {
        if (lists[i].resourceId == resourceId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// If this resource has no queue yet, create one, then enqueue.
void WaitingListManager::enqueue(const WaitRequest& request) {
    int idx = indexOf(request.getResourceId());
    if (idx < 0) {
        Entry e;
        e.resourceId = request.getResourceId();
        e.queue.enqueue(request);
        lists.push_back(e);
    } else {
        lists[idx].queue.enqueue(request);
    }
}

bool WaitingListManager::dequeue(const std::string& resourceId, WaitRequest& out) {
    int idx = indexOf(resourceId);
    if (idx < 0) {
        return false;   // nobody has ever waited for this resource
    }
    return lists[idx].queue.dequeue(out);
}

bool WaitingListManager::peek(const std::string& resourceId, WaitRequest& out) const {
    int idx = indexOf(resourceId);
    if (idx < 0) {
        return false;
    }
    return lists[idx].queue.peek(out);
}

bool WaitingListManager::isEmpty(const std::string& resourceId) const {
    int idx = indexOf(resourceId);
    if (idx < 0) {
        return true;
    }
    return lists[idx].queue.isEmpty();
}

int WaitingListManager::size(const std::string& resourceId) const {
    int idx = indexOf(resourceId);
    if (idx < 0) {
        return 0;
    }
    return lists[idx].queue.size();
}

int WaitingListManager::totalWaiting() const {
    int total = 0;
    for (std::size_t i = 0; i < lists.size(); ++i) {
        total += lists[i].queue.size();
    }
    return total;
}

void WaitingListManager::displayAll() const {
    if (lists.empty() || totalWaiting() == 0) {
        std::cout << "All waiting lists are empty.\n";
        return;
    }

    std::cout << std::left
              << std::setw(12) << "Resource"
              << std::setw(12) << "StudentID"
              << std::setw(22) << "Student Name"
              << std::setw(14) << "Date"
              << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (std::size_t i = 0; i < lists.size(); ++i) {
        if (!lists[i].queue.isEmpty()) {
            std::cout << "Resource " << lists[i].resourceId
                      << " (" << lists[i].queue.size() << " waiting):\n";
            lists[i].queue.displayAll();
        }
    }
}

void WaitingListManager::displayForResource(const std::string& resourceId) const {
    int idx = indexOf(resourceId);
    if (idx < 0 || lists[idx].queue.isEmpty()) {
        std::cout << "Waiting list for " << resourceId << " is empty.\n";
        return;
    }
    lists[idx].queue.displayAll();
}

//old veriosn.

//**bool WaitingListManager::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out.is_open()) {
        return false;
    }
    for (std::size_t i = 0; i < lists.size(); i++) {
        // Make a copy of the queue so we don't destroy the active waitlist
        WaitQueue tempQueue = lists[i].queue;
        WaitRequest req;
        
        // Dequeue one by one and write to file
        while (tempQueue.dequeue(req)) {
            out << req.toFileString() << "\n";
        }
    }
    return true;
}**//

//Updated version.
    
bool WaitingListManager::saveToFile(
    const std::string& filename
) const {
    std::ofstream out(filename.c_str());

    if (!out.is_open()) {
        return false;
    }

    for (std::size_t i = 0; i < lists.size(); ++i) {
        // Copy so saving does not remove real waiting entries.
        WaitQueue tempQueue = lists[i].queue;
        WaitRequest request;

        while (tempQueue.dequeue(request)) {
            out << request.toFileString() << '\n';
        }
    }

    out.close();
    return !out.fail();
}

bool WaitingListManager::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;   // skip blanks and comments
        }
        enqueue(WaitRequest::fromFileString(line));
    }
    return true;
}
