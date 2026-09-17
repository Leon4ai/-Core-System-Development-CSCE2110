#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <iostream>

// Reservation: one confirmed booking.
// Active bookings live in ReservationList (linked list).
// Cancelled bookings are pushed onto CancelStack for undo.
class Reservation {
private:
    int reservationId;        // assigned by ReservationManager, never typed by the user
    int studentId;
    std::string studentName;
    std::string resourceId;   // lookup key into the resource vector
    std::string date;         // YYYY-MM-DD

public:
    Reservation();
    Reservation(int reservationId,
                int studentId,
                const std::string& studentName,
                const std::string& resourceId,
                const std::string& date);

    int getReservationId() const;
    int getStudentId() const;
    std::string getStudentName() const;
    std::string getResourceId() const;
    std::string getDate() const;

    void setReservationId(int id);
    void setStudentId(int id);
    void setStudentName(const std::string& name);
    void setResourceId(const std::string& id);
    void setDate(const std::string& date);

    void display() const;

    // Format: reservationId,studentId,studentName,resourceId,date
    std::string toFileString() const;
    static Reservation fromFileString(const std::string& line);
};

#endif
