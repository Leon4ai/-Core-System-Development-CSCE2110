#ifndef WAITQUEUE_H
#define WAITQUEUE_H


#include "WaitRequest.h"

class WaitQueue {
private:
    struct Node {
        WaitRequest data;
        Node* next;
        
        Node(const WaitRequest& req) {
            data = req;
            next = nullptr;
        }
    };

    Node* front;
    Node* rear;
    int count;
public:
    WaitQueue();
    ~WaitQueue();
    
    // Copy constructor and assignment operator to handle vectors of queues
    WaitQueue(const WaitQueue& other);
    WaitQueue& operator=(const WaitQueue& other);

    //Queue oper
    void enqueue(const WaitRequest& request);
    bool dequeue(WaitRequest& out);
    bool peek(WaitRequest& out) const;
    bool isEmpty() const;
    int size() const;
    
    void displayAll() const;

    // This template allows the lambda function in your saveToFile method to work
    template <typename Func>
    void forEach(Func action) const {
        Node* current = front;
        while (current != nullptr) {
            action(current->data);
            current = current->next;
        }
    }
};

#endif 
