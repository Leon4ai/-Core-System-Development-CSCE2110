#include "CancelStack.h"
#include <iomanip>
#include <iostream>

CancelStack::CancelStack() : topNode(nullptr), count(0) {}

CancelStack::CancelStack(const CancelStack& other)
    : topNode(nullptr), count(0) {
    copyFrom(other);
}

CancelStack& CancelStack::operator=(const CancelStack& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

CancelStack::~CancelStack() {
    clear();
}

// Walk top-to-bottom into an array, then push in reverse so the
// copy ends up with the same top as the original.
void CancelStack::copyFrom(const CancelStack& other) {
    if (other.topNode == nullptr) {
        return;
    }
    const int n = other.count;
    Reservation* items = new Reservation[n];
    Node* current = other.topNode;
    int i = 0;
    while (current != nullptr) {
        items[i++] = current->data;
        current = current->next;
    }
    for (int k = n - 1; k >= 0; --k) {
        push(items[k]);
    }
    delete[] items;
}

void CancelStack::clear() {
    Reservation dummy;
    while (pop(dummy)) {}
}

// New node points at the old top, then becomes the new top. O(1)
void CancelStack::push(const Reservation& reservation) {
    Node* node = new Node(reservation);
    node->next = topNode;
    topNode = node;
    ++count;
}

// Remove the top (most recent cancellation). False if the stack is empty.
bool CancelStack::pop(Reservation& out) {
    if (topNode == nullptr) {
        return false;
    }
    Node* doomed = topNode;
    out = doomed->data;
    topNode = topNode->next;
    delete doomed;
    --count;
    return true;
}

bool CancelStack::peek(Reservation& out) const {
    if (topNode == nullptr) {
        return false;
    }
    out = topNode->data;
    return true;
}

bool CancelStack::isEmpty() const { return topNode == nullptr; }
int CancelStack::size() const { return count; }

void CancelStack::displayAll() const {
    if (isEmpty()) {
        std::cout << "Cancellation history is empty.\n";
        return;
    }
    std::cout << "Most recent cancellation is at the top:\n";
    Node* current = topNode;
    int position = 1;
    while (current != nullptr) {
        std::cout << "  #" << position << "  ResID "
                  << current->data.getReservationId()
                  << "  " << current->data.getStudentName()
                  << "  " << current->data.getResourceId() << "\n";
        current = current->next;
        ++position;
    }
}

