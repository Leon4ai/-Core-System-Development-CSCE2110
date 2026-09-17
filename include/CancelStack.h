
#ifndef CANCEL_STACK_H
#define CANCEL_STACK_H

#include "Reservation.h"

// LIFO stack of cancelled reservations.
// Undo pops the top only — that is the most recent cancellation.
class CancelStack {
private:
    struct Node {
        Reservation data;
        Node* next;
        Node(const Reservation& r) : data(r), next(nullptr) {}
    };

    Node* topNode;   // most recently cancelled booking
    int count;

    void copyFrom(const CancelStack& other);
    void clear();

public:
    CancelStack();
    CancelStack(const CancelStack& other);
    CancelStack& operator=(const CancelStack& other);
    ~CancelStack();

    void push(const Reservation& reservation);  // O(1)
    bool pop(Reservation& out);                 // O(1); false if empty
    bool peek(Reservation& out) const;

    bool isEmpty() const;
    int size() const;
    void displayAll() const;
};

#endif
