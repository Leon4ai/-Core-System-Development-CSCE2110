#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "ResourceManager.h"
#include "ReservationList.h"
#include "WaitingListManager.h"
#include "CancelStack.h"

class ReservationManager {
private:
    ResourceManager resources;

    // ONE linked list for every active reservation.
    ReservationList active;

    // Separate waiting queues and cancellation stack.
    WaitingListManager waiting;
    CancelStack history;

    int nextId;

    bool validRequest(
        int studentId,
        const std::string& studentName,
        const std::string& resourceId,
        const std::string& date
    ) const;

    bool waitingContains(
        int studentId,
        const std::string& resourceId,
        const std::string& date
    ) const;

    bool promote(
        const std::string& resourceId,
        const std::string& date
    );

public:
    ReservationManager() : nextId(1) {
    }

    bool loadResources(const std::string& filename);
    bool loadReservations(const std::string& filename);
    bool loadWaitingLists(const std::string& filename);

    bool saveAll(
        const std::string& resourceFile,
        const std::string& reservationFile,
        const std::string& waitingFile
    ) const;

    int resourceCount() const;
    int activeReservationCount() const;
    int waitingCount() const;

    bool createReservation(
        int studentId,
        const std::string& studentName,
        const std::string& resourceId,
        const std::string& date,
        std::string& message
    );

    bool cancelReservation(
        int reservationId,
        std::string& message
    );

    bool undoCancellation(std::string& message);

    bool removeWaiting(
        int studentId,
        const std::string& resourceId,
        const std::string& date
    );

    bool searchReservationById(
        int reservationId,
        Reservation& result
    ) const;

    void searchReservationsByStudent(int studentId) const;

    bool searchResourceById(
        const std::string& resourceId,
        Resource& result
    ) const;

    bool isAvailable(
        const std::string& resourceId,
        const std::string& date
    ) const;

    void displayAvailability(const std::string& date) const;
    void displayResources() const;
    void displayReservations() const;
    void displayWaitingLists() const;

    void sortResources(int criterion);
    void generateReport() const;
};

#endif
