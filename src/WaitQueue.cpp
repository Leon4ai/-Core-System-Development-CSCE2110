#include "WaitQueue.h"
#include <iostream>
#include <utility>

WaitQueue::WaitQueue()
    : front(nullptr), rear(nullptr), count(0) {
}

WaitQueue::~WaitQueue() {
    WaitRequest request;

    while (dequeue(request)) {
    }
}

WaitQueue::WaitQueue(const WaitQueue& other)
    : WaitQueue() {
    for (Node* current = other.front;
         current != nullptr;
         current = current->next) {
        enqueue(current->data);
    }
}

WaitQueue& WaitQueue::operator=(const WaitQueue& other) {
    if (this != &other) {
        WaitQueue copy(other);

        std::swap(front, copy.front);
        std::swap(rear, copy.rear);
        std::swap(count, copy.count);
    }

    return *this;
}

void WaitQueue::enqueue(const WaitRequest& request) {
    Node* newNode = new Node(request);

    if (rear != nullptr) {
        rear->next = newNode;
    } else {
        front = newNode;
    }

    rear = newNode;
    ++count;
}

bool WaitQueue::dequeue(WaitRequest& out) {
    if (front == nullptr) {
        return false;
    }

    Node* oldFront = front;
    out = oldFront->data;

    front = front->next;
    delete oldFront;

    if (front == nullptr) {
        rear = nullptr;
    }

    --count;
    return true;
}

bool WaitQueue::peek(WaitRequest& out) const {
    if (front == nullptr) {
        return false;
    }

    out = front->data;
    return true;
}

bool WaitQueue::isEmpty() const {
    return front == nullptr;
}

int WaitQueue::size() const {
    return count;
}

void WaitQueue::displayAll() const {
    for (Node* current = front;
         current != nullptr;
         current = current->next) {
        std::cout << current->data.toFileString() << '\n';
    }
}
