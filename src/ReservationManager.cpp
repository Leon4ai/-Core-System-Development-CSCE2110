#include "ReservationManager.h"
#include "Algorithms.h"

#include <algorithm>
#include <climits>
#include <ctime>
#include <fstream>
#include <sstream>

namespace {

// Simple CSV fields must not contain commas or line breaks.
bool textField(const std::string& value) {
    return value.find_first_not_of(" \t\r\n") !=
               std::string::npos &&
           value.find_first_of(",\r\n") ==
               std::string::npos;
}

// Check YYYY-MM-DD, including month lengths and leap years.
bool validDate(const std::string& value) {
    if (value.size() != 10 ||
        value[4] != '-' ||
        value[7] != '-') {
        return false;
    }

    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i != 4 && i != 7) {
            if (value[i] < '0' || value[i] > '9') {
                return false;
            }
        }
    }

    int year = std::stoi(value.substr(0, 4));
    int month = std::stoi(value.substr(5, 2));
    int day = std::stoi(value.substr(8, 2));

    if (year < 1 || month < 1 || month > 12) {
        return false;
    }

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool leapYear =
        year % 400 == 0 ||
        (year % 4 == 0 && year % 100 != 0);

    if (leapYear) {
        daysInMonth[1] = 29;
    }

    return day >= 1 && day <= daysInMonth[month - 1];
}

std::string today() {
    std::time_t currentTime = std::time(nullptr);
    char buffer[11];

    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d",
        std::localtime(&currentTime)
    );

    return buffer;
}

// Remove Windows carriage returns; skip blank/comment lines.
bool skip(std::string& line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    return line.find_first_not_of(" \t") ==
               std::string::npos ||
           line[0] == '#';
}

bool positive(const std::string& value) {
    try {
        std::size_t charactersUsed;
        int number = std::stoi(value, &charactersUsed);

        return charactersUsed == value.size() &&
               number > 0 &&
               number < INT_MAX;
    } catch (...) {
        return false;
    }
}

bool reservationLine(const std::string& line) {
    std::istringstream input(line);

    std::string reservationId;
    std::string studentId;

    std::getline(input, reservationId, ',');
    std::getline(input, studentId, ',');

    return std::count(line.begin(), line.end(), ',') == 4 &&
           positive(reservationId) &&
           positive(studentId);
}

} // namespace

int ReservationManager::resourceCount() const {
    return static_cast<int>(resources.all().size());
}

int ReservationManager::activeReservationCount() const {
    return active.size();
}

int ReservationManager::waitingCount() const {
    return waiting.totalWaiting();
}

bool ReservationManager::validRequest(
    int studentId,
    const std::string& studentName,
    const std::string& resourceId,
    const std::string& date
) const {
    return studentId > 0 &&
           textField(studentName) &&
           resources.resourceExists(resourceId) &&
           validDate(date);
}

bool ReservationManager::isAvailable(
    const std::string& resourceId,
    const std::string& date
) const {
    return resources.resourceExists(resourceId) &&
           validDate(date) &&
           !active.hasConflict(resourceId, date);
}

bool ReservationManager::loadResources(
    const std::string& filename
) {
    std::ifstream input(filename);

    if (!input) {
        return false;
    }

    std::vector<Resource> loaded;
    std::string line;

    while (std::getline(input, line)) {
        if (skip(line)) {
            continue;
        }

        Resource resource = Resource::fromFileString(line);

        if (std::count(line.begin(), line.end(), ',') != 4 ||
            !textField(resource.getId()) ||
            !textField(resource.getName()) ||
            !textField(resource.getType()) ||
            resource.getUsageCount() < 0 ||
            linearSearchResourceById(
                loaded, resource.getId()
            ) >= 0) {
            std::cerr
                << "Invalid/duplicate resource row: "
                << line << '\n';

            return false;
        }

        // Actual availability comes from resource/date
        // conflicts in the active reservation list.
        resource.setAvailable(true);
        loaded.push_back(resource);
    }

    if (loaded.empty() || input.bad()) {
        return false;
    }

    resources.all() = loaded;
    return true;
}

bool ReservationManager::loadReservations(
    const std::string& filename
) {
    std::ifstream input(filename);

    if (!input) {
        return false;
    }

    std::string line;

    while (std::getline(input, line)) {
        if (skip(line)) {
            continue;
        }

        Reservation reservation =
            Reservation::fromFileString(line);

        if (!reservationLine(line) ||
            !validRequest(
                reservation.getStudentId(),
                reservation.getStudentName(),
                reservation.getResourceId(),
                reservation.getDate()
            ) ||
            active.containsId(
                reservation.getReservationId()
            ) ||
            active.hasConflict(
                reservation.getResourceId(),
                reservation.getDate()
            )) {
            std::cerr
                << "Skipped invalid/conflicting reservation: "
                << line << '\n';

            continue;
        }

        active.insert(reservation);

        nextId = std::max(
            nextId,
            reservation.getReservationId() + 1
        );
    }

    return !input.bad();
}

bool ReservationManager::waitingContains(
    int studentId,
    const std::string& resourceId,
    const std::string& date
) const {
    // Copy so searching does not remove real waiting entries.
    WaitingListManager copy = waiting;
    WaitRequest request;

    while (copy.dequeue(resourceId, request)) {
        if (request.getStudentId() == studentId &&
            request.getDate() == date) {
            return true;
        }
    }

    return false;
}

bool ReservationManager::loadWaitingLists(
    const std::string& filename
) {
    std::ifstream input(filename);

    if (!input) {
        return false;
    }

    std::string line;

    while (std::getline(input, line)) {
        if (skip(line)) {
            continue;
        }

        WaitRequest request =
            WaitRequest::fromFileString(line);

        if (std::count(line.begin(), line.end(), ',') != 3 ||
            !validRequest(
                request.getStudentId(),
                request.getStudentName(),
                request.getResourceId(),
                request.getDate()
            )) {
            std::cerr
                << "Skipped invalid waiting row: "
                << line << '\n';

            continue;
        }

        std::string message;

        // If a saved waiting request now has a free slot,
        // create its reservation; otherwise queue it.
        createReservation(
            request.getStudentId(),
            request.getStudentName(),
            request.getResourceId(),
            request.getDate(),
            message
        );
    }

    return !input.bad();
}

bool ReservationManager::createReservation(
    int studentId,
    const std::string& studentName,
    const std::string& resourceId,
    const std::string& inputDate,
    std::string& message
) {
    std::string date =
        inputDate.empty() ? today() : inputDate;

    if (!validRequest(
        studentId,
        studentName,
        resourceId,
        date
    )) {
        message =
            "Invalid request: use a positive student ID, "
            "a name without commas, an existing resource, "
            "and a valid YYYY-MM-DD date.";

        return false;
    }

    std::vector<Reservation> matches(active.size());

    int matchCount = active.findByStudentId(
        studentId,
        matches.data(),
        active.size()
    );

    for (int i = 0; i < matchCount; ++i) {
        if (matches[i].getResourceId() == resourceId &&
            matches[i].getDate() == date) {
            message = "You already have this reservation.";
            return false;
        }
    }

    if (waitingContains(studentId, resourceId, date)) {
        message =
            "You are already waiting for this resource "
            "and date.";

        return false;
    }

    if (!isAvailable(resourceId, date)) {
        waiting.enqueue(
            WaitRequest(
                resourceId,
                studentId,
                studentName,
                date
            )
        );

        message =
            "Resource booked for this date. "
            "Added to FIFO waiting list.";

        return true;
    }

    if (nextId == INT_MAX) {
        message = "Reservation ID limit reached.";
        return false;
    }

    active.insert(
        Reservation(
            nextId++,
            studentId,
            studentName,
            resourceId,
            date
        )
    );

    resources.getResourceById(resourceId)->incrementUsage();

    message =
        "Reservation created. ID: " +
        std::to_string(nextId - 1);

    return true;
}

// Process exactly the original queue length.
// Promote the first student waiting for the freed date.
// Preserve the relative order of all remaining requests.
bool ReservationManager::promote(
    const std::string& resourceId,
    const std::string& date
) {
    int originalSize = waiting.size(resourceId);
    bool promoted = false;
    WaitRequest request;

    for (int i = 0; i < originalSize; ++i) {
        waiting.dequeue(resourceId, request);

        if (!promoted &&
            request.getDate() == date &&
            nextId < INT_MAX) {
            active.insert(
                Reservation(
                    nextId++,
                    request.getStudentId(),
                    request.getStudentName(),
                    resourceId,
                    date
                )
            );

            resources.getResourceById(
                resourceId
            )->incrementUsage();

            promoted = true;
        } else {
            waiting.enqueue(request);
        }
    }

    return promoted;
}

bool ReservationManager::cancelReservation(
    int reservationId,
    std::string& message
) {
    Reservation reservation;

    if (!active.findById(reservationId, reservation)) {
        message = "Reservation not found.";
        return false;
    }

    history.push(reservation);
    active.removeById(reservationId);

    bool promoted = promote(
        reservation.getResourceId(),
        reservation.getDate()
    );

    if (promoted) {
        message =
            "Reservation cancelled. "
            "First waiting student for that date promoted.";
    } else {
        message =
            "Reservation cancelled and added to history.";
    }

    return true;
}

bool ReservationManager::undoCancellation(
    std::string& message
) {
    Reservation reservation;

    if (!history.peek(reservation)) {
        message = "No cancellation to undo.";
        return false;
    }

    if (!isAvailable(
        reservation.getResourceId(),
        reservation.getDate()
    )) {
        message =
            "Cannot undo: that resource/date is booked. "
            "Latest cancellation remains on the stack.";

        return false;
    }

    active.insert(reservation);
    history.pop(reservation);

    message =
        "Restored reservation " +
        std::to_string(reservation.getReservationId()) +
        ".";

    return true;
}

bool ReservationManager::removeWaiting(
    int studentId,
    const std::string& resourceId,
    const std::string& date
) {
    int originalSize = waiting.size(resourceId);
    bool removed = false;
    WaitRequest request;

    for (int i = 0; i < originalSize; ++i) {
        waiting.dequeue(resourceId, request);

        if (!removed &&
            request.getStudentId() == studentId &&
            request.getDate() == date) {
            removed = true;
        } else {
            waiting.enqueue(request);
        }
    }

    return removed;
}

bool ReservationManager::searchReservationById(
    int reservationId,
    Reservation& result
) const {
    return active.findById(reservationId, result);
}

void ReservationManager::searchReservationsByStudent(
    int studentId
) const {
    std::vector<Reservation> found(active.size());

    int count = active.findByStudentId(
        studentId,
        found.data(),
        active.size()
    );

    if (count == 0) {
        std::cout << "No reservations for this student.\n";
    }

    for (int i = 0; i < count; ++i) {
        found[i].display();
    }
}

bool ReservationManager::searchResourceById(
    const std::string& resourceId,
    Resource& result
) const {
    int index = linearSearchResourceById(
        resources.all(),
        resourceId
    );

    if (index < 0) {
        return false;
    }

    result = resources.all()[index];

    result.setAvailable(
        isAvailable(resourceId, today())
    );

    return true;
}

void ReservationManager::displayAvailability(
    const std::string& date
) const {
    if (!validDate(date)) {
        std::cout << "Invalid date. Use YYYY-MM-DD.\n";
        return;
    }

    std::cout
        << "Availability for " << date
        << "\nID        Name                        "
        << "Type                Status        Uses\n";

    for (Resource resource : resources.all()) {
        resource.setAvailable(
            isAvailable(resource.getId(), date)
        );

        resource.display();
    }
}

void ReservationManager::displayResources() const {
    displayAvailability(today());
}

void ReservationManager::displayReservations() const {
    active.displayAll();
}

void ReservationManager::displayWaitingLists() const {
    waiting.displayAll();
}

void ReservationManager::sortResources(int criterion) {
    for (Resource& resource : resources.all()) {
        resource.setAvailable(
            isAvailable(resource.getId(), today())
        );
    }

    quickSortResources(resources.all(), criterion);
    displayResources();
}

void ReservationManager::generateReport() const {
    std::cout
        << "Resources: " << resourceCount()
        << " | Active: " << active.size()
        << " | Waiting: " << waitingCount()
        << '\n';

    displayResources();
    active.displayAll();
    waiting.displayAll();
    history.displayAll();
}

bool ReservationManager::saveAll(
    const std::string& resourceFile,
    const std::string& reservationFile,
    const std::string& waitingFile
) const {
    std::ofstream resourceOutput(resourceFile);
    std::ofstream reservationOutput(reservationFile);

    for (Resource resource : resources.all()) {
        // Retain the legacy file field. Date availability
        // is recalculated from active bookings.
        resource.setAvailable(true);

        resourceOutput
            << resource.toFileString()
            << '\n';
    }

    active.writeTo(reservationOutput);

    resourceOutput.close();
    reservationOutput.close();

    bool waitingSaved = waiting.saveToFile(waitingFile);

    return !resourceOutput.fail() &&
           !reservationOutput.fail() &&
           waitingSaved;
}
