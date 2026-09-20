# Milestone 1 — Complexity Analysis

## Scope and Notation

This report analyzes the current Campus Resource Reservation System implementation. It distinguishes individual data-structure operations from complete operations performed by ReservationManager.

The following symbols are used:

| Symbol | Meaning                                                                               |
| ------ | ------------------------------------------------------------------------------------- |
| N      | Number of active reservations                                                         |
| R      | Number of resources                                                                   |
| W      | Total waiting requests across all resource queues                                     |
| Q      | Waiting requests in the selected resource's queue                                     |
| K      | Number of stored resource-queue entries, treated as at least 1 in complexity formulas |
| H      | Number of cancelled reservations on the history stack                                 |

Stored queue entries can remain after their queues become empty, so K counts stored entries rather than only nonempty queues.

Unless otherwise stated, bounds describe worst-case time. String lengths are treated as bounded. If field lengths grow, string copying, comparison, and output also contribute to the running time.

## 1. Active Reservation Linked List

The system uses one singly linked list for all active reservations. It stores a head pointer but does not maintain a tail pointer.

### Insertion: O(N) time

ReservationList::insert traverses the list to locate the final node before appending the new reservation. Therefore, insertion into a nonempty list takes O(N) time. Inserting into an empty list takes O(1).

Each insertion allocates one node and uses O(1) additional space.

### Removal by ID: O(N) time

ReservationList::removeById searches sequentially for the requested reservation. Once found, it reconnects the surrounding pointers and deletes the node.

The search takes O(N) time in the worst case. Pointer updates and deletion take O(1). Auxiliary space is O(1).

### Search and Conflict Checking: O(N) time

Finding a reservation by ID, finding reservations for a student, and checking for a resource/date conflict each require a sequential traversal.

The list-level search functions use O(1) auxiliary space apart from any caller-provided results storage. The manager's student-search operation allocates a results vector of size N, giving that complete operation O(N) auxiliary space.

### Traversal and Display: O(N) time

Displaying or writing all active reservations visits each node once. Auxiliary space is O(1).

## 2. Complete Reservation Creation

### Time: O(N + R + W + K(Q + 1))

ReservationManager::createReservation performs more work than list insertion alone:

1. It validates the request and searches the resource vector: O(R).
2. It allocates a reservation-results vector and checks the student's active reservations: O(N).
3. It checks for duplicate waiting requests.
4. It searches for an active resource/date conflict: O(R + N), including resource validation.
5. It either appends an active reservation or adds a waiting request.

The duplicate-waiting check copies the entire WaitingListManager, including its queues. This requires O(W + K) time and space.

It then dequeues up to Q requests from the copied target queue. Each manager-level dequeue performs a linear search through the stored queue entries, costing O(K). Including the final unsuccessful lookup, this contributes O(K(Q + 1)) time.

Appending an active reservation takes O(N), and updating the resource's usage count requires another O(R) resource lookup.

Creating a new resource-queue entry can also cause the vector to reallocate and copy existing queues. Its worst-case cost is covered by the W and K terms above.

### Auxiliary Space: O(N + W + K)

The temporary reservation-results vector requires O(N) space, and the copied waiting-list manager requires O(W + K).

## 3. Waiting Queue Operations and Processing

### Queue Primitives: O(1) time

WaitQueue maintains front and rear pointers.

* enqueue attaches a node at the rear.
* dequeue removes the front node.
* peek reads the front node.

Each operation takes O(1) time and O(1) auxiliary space. Enqueue adds one stored node.

### Manager-Level Queue Access: O(K) time

WaitingListManager stores resource queues in a vector. Its indexOf function searches that vector linearly.

Therefore, enqueue into an existing queue, dequeue, peek, and size lookup take O(K) at the manager level, even though the underlying queue operation is O(1).

Adding a previously unseen resource queue can require O(W + K) time in the worst case if vector reallocation copies existing queues.

### Processing Waiters After Cancellation: O(N + R + K(Q + 1)) time

ReservationManager::promote processes exactly the queue's original Q requests.

For each request, it performs a manager-level dequeue and, unless promoted, a manager-level enqueue. Each lookup costs O(K). The first request matching the freed date is promoted, while all remaining requests retain their relative order.

Promotion also appends a reservation to the active list in O(N) time and finds the resource to update its usage count in O(R).

The complete bound is O(N + R + K(Q + 1)). Auxiliary space is O(1), excluding the stored reservation node created by promotion.

### Removing a Waiting Request: O(K(Q + 1)) time

ReservationManager::removeWaiting processes the original queue length, removes the matching request, and re-enqueues the others in their original relative order.

It uses O(1) auxiliary space.

### Displaying Waiting Lists: O(K + W) time

Displaying all waiting lists visits the stored queue entries and their waiting requests. Auxiliary space is O(1).

## 4. Complete Reservation Cancellation

### Time: O(N + R + K(Q + 1))

ReservationManager::cancelReservation:

1. Finds the reservation by ID: O(N).
2. Pushes it onto the cancellation stack: O(1).
3. Removes it from the active list: O(N).
4. Processes waiting requests for the freed resource/date: O(N + R + K(Q + 1)).

Adding these costs gives O(N + R + K(Q + 1)).

Auxiliary space is O(1), excluding newly stored nodes.

## 5. Cancellation Stack and Undo

### Stack Primitives: O(1) time

CancelStack maintains a pointer to the top node.

* push inserts at the top.
* pop removes the top.
* peek reads the top.

Each operation takes O(1) time. A push allocates one node.

### Complete Undo: O(R + N) time

ReservationManager::undoCancellation first peeks at the latest cancelled reservation in O(1).

It then verifies the resource and checks the active linked list for a conflict. These checks take O(R + N).

If the resource/date is free, it appends the reservation to the active linked list in O(N) and pops the stack in O(1).

Therefore, successful undo takes O(R + N) time and O(1) auxiliary space, excluding the restored node. An empty-stack undo takes O(1).

If a conflict exists, the latest cancellation remains on the stack. The program does not skip it or move it to a waiting queue.

### Displaying Cancellation History: O(H) time

Displaying the history visits each stack node once, from newest to oldest. Auxiliary space is O(1).

## 6. Resource Search, Availability, and Sorting

### Resource Lookup: O(R) time

Linear resource lookup scans the resource vector.

The binary-search function takes O(log R) time when its input is already sorted by ID. However, the current menu resource-search operation uses linear search.

### Resource Search Including Availability: O(R + N) time

ReservationManager::searchResourceById finds the resource and calculates its availability for today. Availability checking includes an active-reservation scan, producing an overall O(R + N) bound.

### Displaying Availability for All Resources: O(R² + RN) time

For each of R resources, displayAvailability calls isAvailable.

Each call searches the resource vector in O(R) and checks the reservation list in O(N). Repeating this for all resources gives O(R(R + N)), or O(R² + RN).

### Quick Sort

The Quick Sort algorithm has:

* Average time: O(R log R).
* Worst-case time: O(R²).
* Average recursion space: O(log R).
* Worst-case recursion space: O(R).

The implementation uses the last element as the pivot. Sorted inputs or many equal keys can produce unbalanced partitions.

The complete menu sorting operation also recalculates availability before sorting and displays availability afterward. Including these steps, its worst-case time is O(R² + RN).

## 7. Report Generation and Saving

### Generate Report: O(R² + RN + N + W + K + H) time

generateReport displays resource availability, active reservations, waiting lists, and cancellation history.

Resource availability contributes O(R² + RN). Traversing the remaining structures contributes O(N + W + K + H).

The current report does not perform selection sort or rank resources by usage.

### Save Files: O(R + N + K + W) time

Saving writes every resource, active reservation, and waiting request.

Waiting queues are copied one at a time so that saving does not remove their original entries. If the largest queue contains Qmax requests, peak auxiliary space is O(Qmax).

Cancellation history is not saved between runs.

## 8. Overall Storage

Persistent in-memory storage is O(R + N + W + K + H):

* Resource vector: O(R).
* Active reservation linked list: O(N).
* Waiting-list entries and queue nodes: O(K + W).
* Cancellation stack: O(H).

Reservation creation temporarily uses O(N + W + K) additional space. Other operations have the auxiliary-space costs described above.

## Required Operations Summary

| Required operation                                 | Current implementation       |
| -------------------------------------------------- | ---------------------------- |
| Insert into active linked list                     | O(N) time                    |
| Complete reservation creation                      | O(N + R + W + K(Q + 1)) time |
| Remove from active list by ID                      | O(N) time                    |
| Complete cancellation with waiting-list processing | O(N + R + K(Q + 1)) time     |
| Enqueue/dequeue within one queue                   | O(1) time                    |
| Process waiting list after cancellation            | O(N + R + K(Q + 1)) time     |
| Remove a selected waiting request                  | O(K(Q + 1)) time             |
| Stack push/pop                                     | O(1) time                    |
| Complete undo cancellation                         | O(R + N) time                |

These bounds describe the current implementation, including its searches, copies, and traversals.
