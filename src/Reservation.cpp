#include "Reservation.h"
#include <sstream>
#include <iomanip>

Reservation::Reservation()
    : reservationId(0), studentId(0), studentName(""), resourceId(""), date("") {}

Reservation::Reservation(int reservationId,
                         int studentId,
                         const std::string& studentName,
                         const std::string& resourceId,
                         const std::string& date)
    : reservationId(reservationId), studentId(studentId),
      studentName(studentName), resourceId(resourceId), date(date) {}

int Reservation::getReservationId() const { return reservationId; }
int Reservation::getStudentId() const { return studentId; }
std::string Reservation::getStudentName() const { return studentName; }
std::string Reservation::getResourceId() const { return resourceId; }
std::string Reservation::getDate() const { return date; }

void Reservation::setReservationId(int id) { reservationId = id; }
void Reservation::setStudentId(int id) { studentId = id; }
void Reservation::setStudentName(const std::string& name) { studentName = name; }
void Reservation::setResourceId(const std::string& id) { resourceId = id; }
void Reservation::setDate(const std::string& d) { date = d; }

void Reservation::display() const {
    std::cout << std::left
              << std::setw(10) << reservationId
              << std::setw(12) << studentId
              << std::setw(22) << studentName
              << std::setw(12) << resourceId
              << std::setw(14) << date
              << std::endl;
}

std::string Reservation::toFileString() const {
    std::ostringstream oss;
    oss << reservationId << "," << studentId << "," << studentName
        << "," << resourceId << "," << date;
    return oss.str();
}

// Parse "301,1001,Alice Smith,R205,2026-09-06".
// A broken ID becomes 0 so loadReservations can skip the line.
Reservation Reservation::fromFileString(const std::string& line) {
    std::string idStr, sidStr, name, rid, date;
    std::istringstream iss(line);

    std::getline(iss, idStr, ',');
    std::getline(iss, sidStr, ',');
    std::getline(iss, name, ',');
    std::getline(iss, rid, ',');
    std::getline(iss, date, ',');

    int ridNum = 0;
    int sidNum = 0;
    try { ridNum = std::stoi(idStr); } catch (...) { ridNum = 0; }
    try { sidNum = std::stoi(sidStr); } catch (...) { sidNum = 0; }

    return Reservation(ridNum, sidNum, name, rid, date);
}
