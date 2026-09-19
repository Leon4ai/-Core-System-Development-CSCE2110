//Tom's Versions.
/*
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

#include "ReservationManager.h"
#include "Resource.h"
#include "Reservation.h"

// main.cpp is only the user interface.
// All data-structure work is done inside ReservationManager.

namespace {

const char* RESOURCE_FILE     = "data/resources.txt";
const char* RESERVATION_FILE  = "data/reservations.txt";
const char* WAITING_FILE      = "data/waiting.txt";

// After a failed >> read, clear the error flag and drop the rest of the line.
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Keep asking until the user types an actual integer.
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        std::cout << "Invalid number. Please try again.\n";
        clearInput();
    }
}

// Read a whole line and trim leading/trailing spaces.
std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    std::size_t start = line.find_first_not_of(" \t\r\n");
    std::size_t end = line.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    return line.substr(start, end - start + 1);
}

void printMenu() {
    std::cout << "\n===== Campus Resource Reservation System =====\n";
    std::cout << "1. View Resources\n";
    std::cout << "2. Create Reservation\n";
    std::cout << "3. Cancel Reservation\n";
    std::cout << "4. View Waiting Lists\n";
    std::cout << "5. Undo Cancellation\n";
    std::cout << "6. Search Reservations\n";
    std::cout << "7. Sort Resources\n";
    std::cout << "8. Generate Report\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter Choice: ";
}

void handleCreate(ReservationManager& manager) {
    std::cout << "\n--- Create Reservation ---\n";
    int studentId = readInt("Student ID: ");
    std::string studentName = readLine("Student Name: ");
    std::string resourceId = readLine("Resource ID: ");
    std::string date = readLine("Reservation Date (YYYY-MM-DD, blank = today): ");

    std::string message;
    manager.createReservation(studentId, studentName, resourceId, date, message);
    std::cout << message << "\n";
}

void handleCancel(ReservationManager& manager) {
    std::cout << "\n--- Cancel Reservation ---\n";
    manager.displayReservations();   // show IDs before asking which to cancel
    int id = readInt("Reservation ID: ");
    std::string message;
    manager.cancelReservation(id, message);
    std::cout << message << "\n";
}

void handleUndo(ReservationManager& manager) {
    std::cout << "\n--- Undo Cancellation ---\n";
    std::string message;
    manager.undoCancellation(message);
    std::cout << message << "\n";
}

void handleSearchReservations(ReservationManager& manager) {
    std::cout << "\n--- Search Reservations ---\n";
    std::cout << "1. Search by Reservation ID\n";
    std::cout << "2. Search by Student ID\n";
    std::cout << "3. View all active reservations\n";
    int choice = readInt("Choice: ");

    if (choice == 1) {
        int id = readInt("Reservation ID: ");
        Reservation found;
        if (manager.searchReservationById(id, found)) {
            std::cout << "\nReservation found:\n";
            std::cout << "ResID      StudentID   Student Name          Resource    Date\n";
            std::cout << std::string(70, '-') << "\n";
            found.display();
        } else {
            std::cout << "No reservation with ID " << id << ".\n";
        }
    } else if (choice == 2) {
        int sid = readInt("Student ID: ");
        manager.searchReservationsByStudent(sid);
    } else if (choice == 3) {
        manager.displayReservations();
    } else {
        std::cout << "Invalid search option.\n";
    }
}

void handleSort(ReservationManager& manager) {
    std::cout << "\n--- Sort Resources (Quick Sort) ---\n";
    std::cout << "1. By Resource ID\n";
    std::cout << "2. By Name\n";
    std::cout << "3. By Type\n";
    std::cout << "4. By Availability\n";
    int choice = readInt("Choice: ");
    if (choice < 1 || choice > 4) {
        std::cout << "Invalid sort criterion.\n";
        return;
    }
    manager.sortResources(choice);
}

void handleViewResources(ReservationManager& manager) {
    std::cout << "\n--- Campus Resources ---\n";
    std::cout << "1. View all resources\n";
    std::cout << "2. Search resource by ID\n";
    int choice = readInt("Choice: ");
    if (choice == 2) {
        std::string id = readLine("Resource ID: ");
        Resource found;
        if (manager.searchResourceById(id, found)) {
            std::cout << "\nResource found:\n";
            std::cout << "ID        Name                        Type                Status        Uses\n";
            std::cout << std::string(80, '-') << "\n";
            found.display();
        } else {
            std::cout << "No resource with ID " << id << ".\n";
        }
    } else {
        manager.displayResources();
    }
}

} // namespace

int main() {
    ReservationManager manager;

    // Rebuild memory from the three text files before showing the menu.
    std::cout << "Loading campus resource data...\n";
    if (!manager.loadResources(RESOURCE_FILE)) {
        std::cerr << "Fatal: resources.txt is required. Place it in the data/ folder.\n";
        return 1;
    }
    manager.loadReservations(RESERVATION_FILE);
    manager.loadWaitingLists(WAITING_FILE);
    std::cout << "Loaded " << manager.resourceCount() << " resources, "
              << manager.activeReservationCount() << " reservations, "
              << manager.waitingCount() << " waiting requests.\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid menu selection. Enter a number from the menu.\n";
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
            case 1:
                handleViewResources(manager);
                break;
            case 2:
                handleCreate(manager);
                break;
            case 3:
                handleCancel(manager);
                break;
            case 4:
                std::cout << "\n--- Waiting Lists ---\n";
                manager.displayWaitingLists();
                break;
            case 5:
                handleUndo(manager);
                break;
            case 6:
                handleSearchReservations(manager);
                break;
            case 7:
                handleSort(manager);
                break;
            case 8:
                manager.generateReport();
                break;
            case 9:
                running = false;
                break;
            default:
                std::cout << "Invalid menu selection. Please choose 1-9.\n";
                break;
        }
    }

    // Persist inventory, bookings, and wait lines for the next run.
    std::cout << "\nSaving data...\n";
    if (manager.saveAll(RESOURCE_FILE, RESERVATION_FILE, WAITING_FILE)) {
        std::cout << "All data saved. Goodbye.\n";
    } else {
        std::cout << "Warning: one or more files could not be saved.\n";
    }
    return 0;
}
*/

//Final Version.

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ReservationManager.h"
#include "Resource.h"
#include "Reservation.h"

namespace {

const char* RESOURCE_FILE = "data/resources.txt";
const char* RESERVATION_FILE = "data/reservations.txt";
const char* WAITING_FILE = "data/waiting.txt";

int readInt(const std::string& prompt) {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input closed");
        }

        std::istringstream input(line);
        int value;
        char extra;

        if ((input >> value) && !(input >> extra)) {
            return value;
        }

        std::cout << "Invalid number. Please try again.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;

    std::string line;

    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("Input closed");
    }

    std::size_t start = line.find_first_not_of(" \t\r\n");
    std::size_t end = line.find_last_not_of(" \t\r\n");

    if (start == std::string::npos) {
        return "";
    }

    return line.substr(start, end - start + 1);
}

void printMenu() {
    std::cout
        << "\n===== Campus Resource Reservation System =====\n"
        << "1. View Resources\n"
        << "2. Create Reservation\n"
        << "3. Cancel Reservation\n"
        << "4. View Waiting Lists\n"
        << "5. Undo Cancellation\n"
        << "6. Search Reservations\n"
        << "7. Sort Resources\n"
        << "8. Generate Report\n"
        << "9. Exit\n"
        << "10. Remove Waiting Request\n"
        << "Enter Choice: ";
}

void handleCreate(ReservationManager& manager) {
    std::cout << "\n--- Create Reservation ---\n";

    int studentId = readInt("Student ID: ");
    std::string studentName = readLine("Student Name: ");
    std::string resourceId = readLine("Resource ID: ");

    std::string date = readLine(
        "Reservation Date (YYYY-MM-DD, blank = today): "
    );

    std::string message;

    manager.createReservation(
        studentId,
        studentName,
        resourceId,
        date,
        message
    );

    std::cout << message << '\n';
}

void handleCancel(ReservationManager& manager) {
    std::cout << "\n--- Cancel Reservation ---\n";

    manager.displayReservations();

    int reservationId = readInt("Reservation ID: ");
    std::string message;

    manager.cancelReservation(reservationId, message);

    std::cout << message << '\n';
}

void handleUndo(ReservationManager& manager) {
    std::cout << "\n--- Undo Cancellation ---\n";

    std::string message;
    manager.undoCancellation(message);

    std::cout << message << '\n';
}

void handleSearchReservations(
    ReservationManager& manager
) {
    std::cout
        << "\n--- Search Reservations ---\n"
        << "1. Search by Reservation ID\n"
        << "2. Search by Student ID\n"
        << "3. View all active reservations\n";

    int choice = readInt("Choice: ");

    if (choice == 1) {
        int reservationId = readInt("Reservation ID: ");
        Reservation found;

        if (manager.searchReservationById(
            reservationId, found
        )) {
            std::cout
                << "\nReservation found:\n"
                << "ResID      StudentID   Student Name"
                << "          Resource    Date\n"
                << std::string(70, '-') << '\n';

            found.display();
        } else {
            std::cout
                << "No reservation with ID "
                << reservationId << ".\n";
        }
    } else if (choice == 2) {
        int studentId = readInt("Student ID: ");
        manager.searchReservationsByStudent(studentId);
    } else if (choice == 3) {
        manager.displayReservations();
    } else {
        std::cout << "Invalid search option.\n";
    }
}

void handleSort(ReservationManager& manager) {
    std::cout
        << "\n--- Sort Resources (Quick Sort) ---\n"
        << "1. By Resource ID\n"
        << "2. By Name\n"
        << "3. By Type\n"
        << "4. By Availability\n";

    int choice = readInt("Choice: ");

    if (choice < 1 || choice > 4) {
        std::cout << "Invalid sort criterion.\n";
        return;
    }

    manager.sortResources(choice);
}

void handleViewResources(ReservationManager& manager) {
    std::cout
        << "\n--- Campus Resources ---\n"
        << "1. View all resources\n"
        << "2. Search resource by ID (today)\n"
        << "3. Check availability by date\n";

    int choice = readInt("Choice: ");

    if (choice == 3) {
        std::string date = readLine("Date (YYYY-MM-DD): ");
        manager.displayAvailability(date);
    } else if (choice == 2) {
        std::string resourceId = readLine("Resource ID: ");
        Resource found;

        if (manager.searchResourceById(
            resourceId, found
        )) {
            std::cout
                << "\nResource found:\n"
                << "ID        Name                        "
                << "Type                Status        Uses\n"
                << std::string(80, '-') << '\n';

            found.display();
        } else {
            std::cout
                << "No resource with ID "
                << resourceId << ".\n";
        }
    } else {
        manager.displayResources();
    }
}

} // namespace

int main() {
    ReservationManager manager;

    std::cout << "Loading campus resource data...\n";

    if (!manager.loadResources(RESOURCE_FILE)) {
        std::cerr
            << "Fatal: resources.txt is required. "
            << "Place it in the data/ folder.\n";

        return 1;
    }

    manager.loadReservations(RESERVATION_FILE);
    manager.loadWaitingLists(WAITING_FILE);

    std::cout
        << "Loaded " << manager.resourceCount()
        << " resources, "
        << manager.activeReservationCount()
        << " reservations, "
        << manager.waitingCount()
        << " waiting requests.\n";

    bool running = true;

    while (running) {
        printMenu();

        try {
            int choice = readInt("");

            switch (choice) {
                case 1:
                    handleViewResources(manager);
                    break;

                case 2:
                    handleCreate(manager);
                    break;

                case 3:
                    handleCancel(manager);
                    break;

                case 4:
                    std::cout << "\n--- Waiting Lists ---\n";
                    manager.displayWaitingLists();
                    break;

                case 5:
                    handleUndo(manager);
                    break;

                case 6:
                    handleSearchReservations(manager);
                    break;

                case 7:
                    handleSort(manager);
                    break;

                case 8:
                    manager.generateReport();
                    break;

                case 9:
                    running = false;
                    break;

                case 10: {
                    int studentId = readInt("Student ID: ");

                    std::string resourceId =
                        readLine("Resource ID: ");

                    std::string date =
                        readLine("Date (YYYY-MM-DD): ");

                    bool removed = manager.removeWaiting(
                        studentId,
                        resourceId,
                        date
                    );

                    std::cout
                        << (removed
                            ? "Waiting request removed.\n"
                            : "Waiting request not found.\n");

                    break;
                }

                default:
                    std::cout
                        << "Invalid menu selection. "
                        << "Please choose 1-10.\n";

                    break;
            }
        } catch (const std::runtime_error&) {
            running = false;
        }
    }

    std::cout << "\nSaving data...\n";

    if (manager.saveAll(
        RESOURCE_FILE,
        RESERVATION_FILE,
        WAITING_FILE
    )) {
        std::cout << "All data saved. Goodbye.\n";
    } else {
        std::cout
            << "Warning: one or more files "
            << "could not be saved.\n";
    }

    return 0;
}

