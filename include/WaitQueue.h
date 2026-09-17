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
    
   
    WaitQueue(const WaitQueue& other);
    WaitQueue& operator=(const WaitQueue& other);

    // Queue oper
    void enqueue(const WaitRequest& request);
    bool dequeue(WaitRequest& out);
    bool peek(WaitRequest& out) const;
    bool isEmpty() const;
    int size() const;
    
    void displayAll() const;
};

#endif 
