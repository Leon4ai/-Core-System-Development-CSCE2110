#ifndef RESERVATIONLIST_H
#define RESERVATIONLIST_H
#include "Reservation.h"

class ReservationList {
private:
    // nested node struct
    struct Node {
        Reservation data;
        Node* next;
        
        Node(const Reservation& res) : data(res), next(nullptr) {}
    };

    Node* head;
    int count;
    // helper functions
    void copyFrom(const ReservationList& other);
    void clear();

public:
    ReservationList();
    ReservationList(const ReservationList& other);
    ReservationList& operator=(const ReservationList& other);
    ~ReservationList();

    // Operations
    void insert(const Reservation& reservation);
    bool removeById(int reservationId);
    
    // Search and Validation
    bool findById(int reservationId, Reservation& out) const;
    int findByStudentId(int studentId, Reservation* results, int maxResults) const;
    bool containsId(int reservationId) const;
    
    // Status and Display
    bool isEmpty() const;
    int size() const;
    void displayAll() const;
};

#endif
