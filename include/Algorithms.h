#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

#include "Resource.h"

int linearSearchResourceById(
    const std::vector<Resource>& resources,
    const std::string& resourceId
);

int binarySearchResourceById(
    const std::vector<Resource>& resources,
    const std::string& resourceId
);

void quickSortResources(
    std::vector<Resource>& resources,
    int criterion
);

#endif
