# Milestone 1 — Complexity Analysis

## Scope and Notation

This report is an analysis of the existing Campus Resource Reservation System implementation. It separates operations on the individual data-structures from the full operations of ReservationManager.

Symbols used:

| Symbol | Meaning                                                                               |
| ------ | ------------------------------------------------------------------------------------- |
| N      | Number of active reservations                                                         |
| R      | Number of resources                                                                   |
| W      | Total waiting requests across all resource queues                                     |
| Q      | Waiting requests in the selected resource's queue                                     |
K >= 1 (in complexity formulas) | Number of stored resource-queue entries |
| H      | Number of cancelled reservations on the history stack                                 |

Queues may become empty while still having stored elements in their queues, so K counts stored elements and not only the empty queues.

Unless otherwise indicated, bounds refer to worst case time. String lengths are considered to be bounded. The running time also increases as the length of the fields increases, because of copying, comparison and output of strings.

## 1. Active Reservation Linked List

The system has a single linked list of all the active reservations. It keeps track of a head pointer, but not a tail pointer.

### Insertion: O(N) time

The function ReservationList::insert searches the list to find the last node to insert the new reservation into. Thus, the insertion into a nonempty list is O(n). O(1) for inserting into an empty list.

One node is allocated on each insertion, and space is used is O(1).

### Removal by ID: O(N) time

ReservationList::removeById will search sequentially for the reservation to be removed. This will connect the surrounding pointers when it is located and remove the node.

In the worst case, the search has time complexity of O(N). Pointer update and deletion is O(1). The auxiliary space is O(1).

This method takes O(N) time to search for conflicts.Searching for conflicts in this method takes O(N) time.

Sequential traversal is needed to find a reservation by ID, to find reservations for a student, and to check for a resource/date conflict.

The list-level search functions requires O(1) extra space besides any storage provided by the caller on their part. The student-search operation of the manager uses N auxiliary space because it returns a results vector of size N.

To traverse and display, it takes O(N) time.It takes O(N) time to traverse and display.

This view or writing of all active reservations traverses each node exactly once. The auxiliary space is O(1).

## 2. Complete Reservation Creation

### Time: O(N + R + W + K(Q + 1))

ReservationManager::createReservation does more than just list insertion:

It validates the request and searches the resource vector: O(R).
It sets up a reservation-results vector and queries the student's active reservations: O(N).
3. Verifies duplicate waiting requests.
4) Looks for an active resource/date conflict: O(R + N) - with resource validation.
Either it adds a reservation to the end or it makes a waiting request.

The duplicate-waiting check is a copy of the full WaitingListManager, which contains its queues. O(W + K) time and space are required.

Then it retrieves up to Q requests from the duplicated target queue. A linear search is made at the manager level through the stored queue entries, which takes O(K) where K is the number of entries in the queue. This takes O(K(Q + 1)) time, including the last failed lookup.

Adding and updating resource usage count are both O(N) and O(R) operations, respectively.

Another resource-queue entry can be created, which may cause the vector to reallocate and duplicate existing queues. The worst case scenario is dealt with by the following two terms, W and K.

### Auxiliary Space: O(N + W + K)

The temporary reservation-results vector takes O(N) space, and the copy of the waiting-list manager takes O(W + K) space.

## 3. Determine the time to wait in a waiting queue and the processing time.

### Queue Primitives: O(1) time

WaitQueue has front and rear pointers.

enqueue puts a node on the end.
Remove the front node from the queue: dequeue.
peek reads the front node (last to be added).

The running time and auxiliary space of each of the operations is O(1). The node is added to the queue by the enqueue operation.

Access to queues is performed in an O(K) time.Access to queues is done in O(K) time.

Queues of resources are kept in the WaitingListManager using a vector. That vector is linearly searched by its indexOf function.

Hence, the manager-level enqueue, dequeue, peek or size lookup are all O(K).

If the vector is reallocated to add the new resource queue, this can take O(W + K) time in the worst case.

Since the waiters are being processed after cancelation, the complexity of the processing becomes O(N + R + K(Q + 1)).

ReservationManager::promote processes exactly the queue's original Q requests.

It does a manager level dequeue and, unless promoted, it does a manager level enqueue for each request. The cost of each look-up is O(K). All requests are queued and the first one that matches the freed date is promoted, the rest being kept in their relative order.

Promotion also adds a reservation to the active list in O(N) time and determines which resource it is to change its usage count in O(R).

The complete bound is O(N + R + K(Q + 1)). The number of auxiliary spaces is O(1) (excluding the reservation node stored as a result of promotion).

To remove a waiting request can take O(Q + 1) time.Deleting a waiting request takes O(Q + 1) time.

ReservationManager::removeWaiting acts on the original length of the queue, deletes the matching queue element and restores the remaining elements in the original relative order.

Uses O(1) aux space.

Displaying waiting lists is a O(K + W) time operation.The operation of displaying waiting lists takes O(K + W) time.

Shows all the waiting lists stored in the queue and the waiting requests for them. The auxiliary space required is O(1).

## 4. Complete Reservation Cancellation

### Time: O(N + R + K(Q + 1))

ReservationManager::cancelReservation:

2. Computes the value of the function at a given point x: O(1)
2. Places it on the cancellation stack: O(1).
3. Takes it out of the list of active items: O(N).
4. Queues requests for the freed resource/date: O(N + R + K(Q + 1)).

Adding these costs gives O(N + R + K(Q + 1)).

Auxiliary space is O(1) (excluding nodes being added).

## 5. Can cancel out and undo operations.

The time is O(1).Time is constant, O(1).

The pointer points to the top node in CancelStack.

* push inserts at the top.
To remove the top, use pop.
* peek reads the top.

All operations will run in O(1) time. One node is allocated by a push.

### Complete Undo: O(R + N) time

The doUndoCancellation method in ReservationManager makes an O(1) initial call to the latest cancelled reservation.

It then checks if the resource is verified and if there is any conflict in the active linked list. The checks are O(R + N).

If the resource/date available, it adds the resource to the active linked list in O(N) time and removes the stack in O(1) time.

So the time required for successful undo is O(R + N) and space used during undo is O(1) (excluding the restored node). The time complexity of undoing an empty stack is O(1).

In the event of a conflict, the newest cancelation will stay on the stack. It is not skipped or placed in a waiting queue by the program.

The Cancellation History procedure executes in O(H) time.The Cancellation History procedure runs in linear time O(H).

History prints each stack node exactly once; in reverse order. The auxiliary space required is O(1).

## 6. The process of searching, locating, and organizing resources.

Searching a set of resources within O(R) time.Searching a set of resources in O(R) time.

Linear resource lookup: Scans resource vector.

If the function is called with a sorted (by ID) input, its runtime is O(log R). The menu resource-search operation is currently performed using linear search, however.

The time required for Resource Search is O(R + N) time.The time taken for ResourceSearch is O(R + N) time.

ReservationManager::searchResourceById gets the resource and computes its availability for today. Availability checking is performed by active-reservation scan and hence gives an overall bound of O(R + N).

To display the availability for all resources takes O(R² + RN) time.Displaying the availability for all resources takes O(R² + RN) time.

For each of R resources, displayAvailability calls isAvailable.

Time to search the resource vector is O(R), then a check of the reservation list is O(N) for each call. Doing this for every resource will result in O(R(R + N)), or O(R² + RN).

### Quick Sort

Quick Sort algorithm has:

The average time is O(R log R).
* Worst-case time: O(R²).
Average space used for recursion: O(log R).
* Worst case running time: O(log n).

The implementation is based on the pivot being the last element. Unbalanced partitions are possible with sorted inputs or with many equal keys.

The full menu sort also calculates the availability before sorting and shows availability after sorting. With these steps, its worst case time is O(R² + RN).

## 7. Create and Save Reports

### Generate Report: O(R² + RN + N + W + K + H) time

generateReport shows resource availability, active reservations, waiting lists and cancellations.

O(R² + RN) is for resource availability. Walking through the rest of the structures adds O(N + W + K + H).Walking through the rest of the structures adds O(N + W + K + H).

This report does not sort or rank resources by usage, nor does it use selection sort.

### Save Files: O(R + N + K + W) time

Saving saves all resources, reservations and waiting requests.

When waiting queues are saved, they are saved one at a time, without losing the contents of the original queues. The worst case is O(Qmax) when the biggest queue is populated with Qmax requests.

Cancellation history will not be stored between runs.

## 8. Overall Storage

Persistent in-memory storage: O(R + N + W + K + H):

* Resource vector: O(R).
O(N) - Active reservation linked list.
Waiting-list entries and queue nodes: O(K + W).
* Cancellation stack: O(H).

Creating the reservations requires N + W + K extra space to be used temporarily. The other operations have the following auxiliary-space costs.

## Required Operations Summary

| Required operation                                 | Current implementation       |
| -------------------------------------------------- | ---------------------------- |
| Insert into active linked list                     | O(N) time                    |
| Complete reservation creation                      | O(N + R + W + K(Q + 1)) time |
| Remove from active list by ID                      | O(N) time                    |
Waiting-list processing: Cancel all the blocks (O(N + R + K(Q + 1) time)
| Enqueue/dequeue within one queue                   | O(1) time                    |
| Process waiting list after cancellation            | O(N + R + K(Q + 1)) time     |
| Remove a selected waiting request                  | O(K(Q + 1)) time             |
| Stack push/pop                                     | O(1) time                    |
| Complete undo cancellation                         | O(R + N) time                |

These bounds outline the current implementation which encompasses searching, copying and traversing.
