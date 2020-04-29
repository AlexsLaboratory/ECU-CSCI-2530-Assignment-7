// CSCI 2530
// Assignment: 6
// Author:     Alex Lowe
// File:       pqueue.cpp
// Tab stops:  None

/*
 * pqueue.cpp is a priority queue which is collection of items that each have
 * a priority. Each collection has the name of the item and then a double
 * represents the priority of that item.
 * There are a few key operations that priority queue supports:
 * 1. You can ask a priority queue whether it is empty.
 * 2. You can insert an item into the priority queue with a name and a given
 * priority.
 * 3. You can remove the item from the priority queue tha has the smallest
 * priority.
 *
 * For example, suppose that you start with an empty priority queue and imagine performing the following steps.
 *     1. Insert item "one" with priority 10.
 *     2. Insert item "two" with priority 5.
 *     3. Insert item "three" with priority 15.
 *     4. Remove the item with the smallest priority and call it x.
 *     5. Remove the item with the smallest priority and call it y.
 * You should find that x is "two" and y is "one".
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "pqueue.h"
using namespace std;

// A structure called PQCell that takes in an item, priority, and the next cell.

struct PQCell {
    ItemT item;
    PriorityT priority;
    PQCell* next;

    PQCell(ItemT it, PriorityT pri, PQCell* nxt) {
      item = it;
      priority = pri;
      next = nxt;
    }
};

// InsertCell(L, i, p) inserts a cell with item i and priority p into linked
// list L. It assumes that L is in non-descending order by priority, and it
// inserts the new cell in a place so that L is still in non-descending order
// by priority.

void insertCell(PQCell*& L, ItemT item, PriorityT priority) {
 if (L == NULL || priority <= L->priority)
 {
    L = new PQCell(item, priority, L);
 }
 else
 {
   insertCell(L->next, item, priority);
 }
}

// isEmpty(q) returns true if q is empty and returns false if q is not empty.

bool isEmpty(const PriorityQueue& q) {
  return q.head == NULL;
}

// insert(q, i, p) inserts item i, with priority p, into q.

void insert(PriorityQueue& q, ItemT i, PriorityT p) {
  insertCell(q.head, i, p);
}

// If queue is empty then print message if queue is not empty then removes
// the item from q that has the smallest priority. Parameters i and p are
// out-parameters, an out-parameter represents information that is passed
// from the function back to its caller. "remove" stores the removed item
// into variable i and stores the removed item's priority into variable p

void remove(PriorityQueue& q, ItemT& i, PriorityT& p) {
  if (isEmpty(q))
  {
    cout << "Attempt to remove from an empty priority queue" << endl;
    exit(1);
  }
  
  i = q.head->item;
  p = q.head->priority;
  
  PQCell* nextCell = q.head->next;
  delete q.head;
  
  q.head = nextCell;
}

// Print the current contents of q. Use printItem(i) to print item i and use printPriority(p) to print priority p.

void ShowPriorityQueue(const PriorityQueue& q, ItemPrinterT printItem,
                       PriorityPrinterT printPriority) {
  for(const PQCell* p = q.head; p != NULL; p = p->next)
  {
    cout << "Item: ";
    printItem(p->item);
    cout << " Priority: ";
    printPriority(p->priority);
    cout << endl;
  }
}