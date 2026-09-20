# -Core-System-Development-CSCE2110
Incorporating resource management, reservations and waiting lists and recording cancellation history for Campus Resource Reservation System (CRRS), using linked lists, queues and stacks.

Campus Resource Reservation System
==================================

A menu-driven C++ command-line application that lets a university manage
campus resources (study rooms, laptops, calculators, lab equipment, and
tutoring appointments). Students can reserve resources, join waiting
lists when an item is taken, cancel reservations, and undo the most
recent cancellation. Administrators can search, sort, and generate
usage reports.

This project demonstrates object-oriented design and the required
fundamental data structures and algorithms.


How to Build and Run
--------------------
From the Project1 directory:

    make
    ./campus_reservations

Or compile by hand:

    g++ -std=c++11 -Wall -Iinclude src/*.cpp -o campus_reservations
    ./campus_reservations

The program expects its data files to live in the data/ folder relative
to the working directory, so run it from the Project1 root.


Menu
----
===== Campus Resource Reservation System =====
1. View Resources
2. Create Reservation
3. Cancel Reservation
4. View Waiting Lists
5. Undo Cancellation
6. Search Reservations
7. Sort Resources
8. Generate Report
9. Exit

View Resources also lets you search by resource ID.
Search Reservations also lets you list every active reservation.


Data Files
----------
data/resources.txt
    Format: ResourceID,Name,Type,Available(1/0),UsageCount

data/reservations.txt
    Format: ReservationID,StudentID,StudentName,ResourceID,Date

data/waiting.txt
    Format: ResourceID,StudentID,StudentName,Date

Files are loaded at startup and saved automatically on exit.
Lines that start with '#' are treated as comments.


Design Overview
---------------
Class                 Role
-------------------   ----------------------------------------------
Resource              One campus resource (id, name, type, status)
Reservation           One booking (ids, student, date)
Student               Lightweight student record
WaitRequest           One waiting-list entry
ReservationList       Singly linked list of active reservations
WaitQueue             FIFO linked-list queue
CancelStack           LIFO linked-list stack
WaitingListManager    One queue per resource ID
ReservationManager    Coordinates all operations and file I/O
Algorithms            Linear search, binary search, Quick Sort


Required Data Structures
------------------------
Linked List  Active reservations.
             Frequent insert/delete and a changing size make a list
             a better fit than a fixed array.

Queue        Per-resource waiting lists.
             First-come, first-served matches FIFO.

Stack        Cancellation history.
             Undo restores only the most recent cancellation (LIFO).

Vector       Resource inventory.
             Fast sequential access and in-place Quick Sort.


Required Algorithms
-------------------
Linear Search   Find a resource or reservation by ID; find every
                reservation belonging to a student. O(n).

Binary Search   Implemented for resources after they are sorted by
                ID. O(log n).

Quick Sort      Sorts resources by ID, name, type, or availability.
                Average O(n log n), extra memory O(log n) for recursion.


Reservation Rules
-----------------
* Creating a reservation against an available resource marks that
  resource reserved and increments its usage count.
* Creating a reservation against an unavailable resource places the
  student on that resource's FIFO waiting list.
* Cancelling a reservation pushes it onto the undo stack. If anyone is
  waiting for that resource, the next student is assigned automatically.
* Undo pops the stack and restores the reservation when the resource is
  free. If the resource is already taken, the request is queued again.
* Duplicate / unknown IDs and empty stack / empty queue cases are
  rejected with a clear message.


File Organization
-----------------
Project1/
├── include/          Public headers
├── src/              Implementations + main.cpp
├── data/             Sample input files
├── Makefile
├── README.txt
└── .gitignore


Efficiency Notes
----------------
Operation                         Structure / Algorithm      Complexity
--------------------------------  -------------------------  ----------
Insert reservation                Linked list tail insert    O(n)
Cancel reservation by ID          Linked list scan + delete  O(n)
Search reservation by ID          Linear search              O(n)
Search reservations by student    Linear scan                O(n)
Search resource by ID             Linear (or binary) search  O(n) / O(log n)
Sort resources                    Quick Sort                 O(n log n) avg
Enqueue / dequeue waiting list    Queue                      O(1)
Push / pop cancellation           Stack                      O(1)
Generate frequency report         Copy + selection sort      O(n^2) on n resources


Sample Session
--------------
Create Reservation
Student ID: 1001
Student Name: Alice Smith
Resource ID: R205
Reservation Created Successfully.

Cancel Reservation
Reservation ID: 301
Reservation Cancelled.
Added to cancellation history.

Undo Cancellation
Reservation Restored Successfully.

## Additional Usage and Implementation Notes

The menu also includes:

10. Remove Waiting Request

This option asks for the student ID, resource ID, and requested date.

View Resources includes a "Check availability by date" option. Resource
search and the default resource display show availability for today.

There is exactly ONE linked list containing all active reservations
across the system. Each reservation stores its reservation ID, student
ID, student name, resource ID, and date. Availability is determined by
traversing this list for a matching resource ID and date. The same
resource can therefore have reservations on different dates.

Waiting queues are separate from the active reservation list. Each
resource has its own queue. When a reservation is cancelled, the first
waiting request for the freed date is promoted. The relative order of
all remaining requests is preserved.

The cancellation stack is separate from both active reservations and
waiting queues. Generate Report displays cancellation history, with
the most recent cancellation first. Cancellation history exists only
during the current run; it is not saved between runs.

The Available field in resources.txt is retained for file-format
compatibility. Actual booking availability is calculated from active
reservations for the requested date.

Reservation dates must be valid calendar dates in YYYY-MM-DD format.
A blank date during reservation creation uses today's date. The sample
session above is abbreviated and assumes initially empty reservation
and waiting files, a blank date entry, and no waiting students.

The efficiency table uses:

* N: number of active reservations.
* R: number of resources.
* W: total number of waiting requests.
* Q: number of waiting requests for the selected resource.
* K: number of stored resource-queue entries, treated as at least 1
  when expressing complexity bounds.
* H: number of cancelled reservations currently on the stack.

Queue enqueue/dequeue and stack push/pop are O(1) primitive operations.
Complete system operations can take longer because they also perform
resource searches, reservation scans, queue lookup, or queue copying.
See docs/complexity-analysis.md for the complete analysis.

The project also contains tests/test_system.py. From the project root,
run the included checks with:

```
python3 tests/test_system.py
```

The test script builds the program using make and runs its scenarios
in temporary directories.

Before submission, build and run the project on a UNT CSE CELL machine
from the directory containing Makefile and data/:

```
make clean
make
./campus_reservations
```

