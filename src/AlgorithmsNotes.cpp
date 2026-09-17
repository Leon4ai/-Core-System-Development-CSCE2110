#include "Algorithms.h"

// Linear search: check every resource until the ID matches.
// Works whether or not the vector is sorted. O(n)
int linearSearchResourceById(const std::vector<Resource>& resources,
                             const std::string& resourceId) {
    for (std::size_t i = 0; i < resources.size(); ++i) {
        if (resources[i].getId() == resourceId) {
            return static_cast<int>(i);
        }
    }
    return -1;   // not found
}

// Binary search: repeatedly cut the remaining range in half.
// PRECONDITION: resources must already be sorted by ID. O(log n)
int binarySearchResourceById(const std::vector<Resource>& resources,
                             const std::string& resourceId) {
    int low = 0;
    int high = static_cast<int>(resources.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        const std::string& midId = resources[mid].getId();
        if (midId == resourceId) {
            return mid;
        }
        if (midId < resourceId) {
            low = mid + 1;    // target is in the right half
        } else {
            high = mid - 1;   // target is in the left half
        }
    }
    return -1;
}

// Decide ordering for Quick Sort based on the user's menu choice.
static int compareResources(const Resource& a, const Resource& b, int criterion) {
    switch (criterion) {
        case 2:  // name A-Z
            if (a.getName() < b.getName()) return -1;
            if (a.getName() > b.getName()) return 1;
            return 0;
        case 3:  // type A-Z
            if (a.getType() < b.getType()) return -1;
            if (a.getType() > b.getType()) return 1;
            return 0;
        case 4:  // reserved rows first, then available
            if (a.isAvailable() == b.isAvailable()) return 0;
            return a.isAvailable() ? 1 : -1;
        case 1:  // ID
        default:
            if (a.getId() < b.getId()) return -1;
            if (a.getId() > b.getId()) return 1;
            return 0;
    }
}

// Place the pivot (last element) in its final sorted position.
// Everything <= pivot ends up on the left.
static int partition(std::vector<Resource>& resources, int low, int high, int criterion) {
    Resource pivot = resources[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (compareResources(resources[j], pivot, criterion) <= 0) {
            ++i;
            Resource tmp = resources[i];
            resources[i] = resources[j];
            resources[j] = tmp;
        }
    }
    Resource tmp = resources[i + 1];
    resources[i + 1] = resources[high];
    resources[high] = tmp;
    return i + 1;   // pivot index
}

// Recurse on the left slice and the right slice.
static void quickSortRange(std::vector<Resource>& resources, int low, int high, int criterion) {
    if (low < high) {
        int p = partition(resources, low, high, criterion);
        quickSortRange(resources, low, p - 1, criterion);
        quickSortRange(resources, p + 1, high, criterion);
    }
}

void quickSortResources(std::vector<Resource>& resources, int criterion) {
    if (resources.size() < 2) {
        return;   // already sorted
    }
    quickSortRange(resources, 0, static_cast<int>(resources.size()) - 1, criterion);
}
