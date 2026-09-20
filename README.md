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

Following are additional usage and implementation notes for this module:

There is also a menu of:

10. Remove Waiting Request

This option requests the student ID, resource ID and desired date.

View Resources has a "Check availability by date" feature. Resource
Availability is indicated in the search and default resource display for today.

There is EXACTLY 1 linked list that is used for all reservations that are currently active
across the system. Reservation ID and student information in a separate table.Each Reservation has its own Reservation ID and Student information in a different table.
ID, Student Name, Resource ID, Date. Availability is based on:
Walking through this list looking for a resourceID and date match. The same
resource can therefore have reservations on different dates.

The waiting queue is a separate list from the active reservation list. Each
There is a queue associated with resource. When a reservation is cancelled, the first
waiting request for the freed date is promoted. The relative order (SOV) of words in a sentence.
all other requests is saved.

and the number of reservations conducted via the cancel command is not recorded in the cancellation stack.
waiting queues. Generate Report shows a cancellation history of
the most recent cancellation first. There is only one event with the given name.
It is not saved between runs, but rather during the current run.

The Available field in resources.txt is carried over for file-format
compatibility. To calculate actual booking availability, the term "active" is referenced to.
Bookings for the desired date.

Reservations should be calendar dates (YYYY-MM-DD).
If there is a blank date during the reservation creation, the date is set to the "Today". The sample
The session above is shortened and assumes that the reservations are empty at the start.
and waiting files, blank date entry and no waiting students.

The efficiency table applies to:

* N: number of available reservations.
R: Number of resources.
W: Sum of all the waiting requests.
Awaits = number of requests waiting for the selected resource.
Note: K is the minimum value of the number of resource-queue entries stored, at least 1.
  when expressing complexity bounds.
H: number of places currently on the reservation stack that have been cancelled.

Primitive operations for queue enqueue/dequeue and stack push/pop are O(1).
Complete system operations may take longer as it also performs
This could be via resource searches, reservation scans, queue lookup or queue copying.
For the full analysis, please see docs/complexity-analysis.md.

The project has a test/test_system.py file as well. To the project root,
Perform the tests provided with:

```
python3 tests/test_system.py
```

The test script uses make to compile the program and executes its scenarios.
in temporary directories.

Build and run the project on UNT CSE CELL machine before submission
Remove the files from the directory containing Makefile and data/:

```
make clean
make
./campus_reservations
```
```
