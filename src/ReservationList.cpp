#include "ReservationList.h"
#include <iomanip>
#include <iostream>

ReservationList::ReservationList() : head(nullptr), count(0) {}

// Deep copy: clone every node so the two lists do not share memory.
ReservationList::ReservationList(const ReservationList& other)
    : head(nullptr), count(0) {
    copyFrom(other);
}

ReservationList& ReservationList::operator=(const ReservationList& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

// Walk the chain and delete every node to avoid leaks.
ReservationList::~ReservationList() {
    clear();
}

void ReservationList::copyFrom(const ReservationList& other) {
    Node* current = other.head;
    Node* tail = nullptr;
    while (current != nullptr) {
        Node* node = new Node(current->data);
        if (head == nullptr) {
            head = node;          // first cloned node becomes the new head
        } else {
            tail->next = node;    // attach onto the growing copy
        }
        tail = node;
        current = current->next;
        ++count;
    }
}

void ReservationList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* doomed = current;
        current = current->next;
        delete doomed;
    }
    head = nullptr;
    count = 0;
}

// Tail insert keeps bookings in the order they were created.
void ReservationList::insert(const Reservation& reservation) {
    Node* node = new Node(reservation);
    if (head == nullptr) {
        head = node;                  // list was empty
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;  // walk to the last node
        }
        current->next = node;
    }
    ++count;
}

// Linear scan. Relink the previous node around the match, then delete it.
bool ReservationList::removeById(int reservationId) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.getReservationId() == reservationId) {
            if (previous == nullptr) {
                head = current->next;             // deleted the first node
            } else {
                previous->next = current->next;   // skip over current
            }
            delete current;
            --count;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;   // ID was not in the list
}

// Linear search by reservation ID. Copy the match into 'out'.
bool ReservationList::findById(int reservationId, Reservation& out) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.getReservationId() == reservationId) {
            out = current->data;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Collect every booking that belongs to one student (linear scan).
int ReservationList::findByStudentId(int studentId,
                                     Reservation* results,
                                     int maxResults) const {
    int found = 0;
    Node* current = head;
    while (current != nullptr && found < maxResults) {
        if (current->data.getStudentId() == studentId) {
            results[found] = current->data;
            ++found;
        }
        current = current->next;
    }
    return found;
}

bool ReservationList::containsId(int reservationId) const {
    Reservation dummy;
    return findById(reservationId, dummy);
}

bool ReservationList::isEmpty() const { return head == nullptr; }
int ReservationList::size() const { return count; }

void ReservationList::displayAll() const {
    if (isEmpty()) {
        std::cout << "No active reservations.\n";
        return;
    }

    std::cout << std::left
              << std::setw(10) << "ResID"
              << std::setw(12) << "StudentID"
              << std::setw(22) << "Student Name"
              << std::setw(12) << "Resource"
              << std::setw(14) << "Date"
              << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    Node* current = head;
    while (current != nullptr) {
        current->data.display();
        current = current->next;
    }
}

bool ReservationList::hasConflict(
    const std::string& resource,
    const std::string& date
) const {
    for (Node* current = head;
         current != nullptr;
         current = current->next) {
        if (current->data.getResourceId() == resource &&
            current->data.getDate() == date) {
            return true;
        }
    }

    return false;
}

void ReservationList::writeTo(std::ostream& out) const {
    for (Node* current = head;
         current != nullptr;
         current = current->next) {
        out << current->data.toFileString() << '\n';
    }
}
