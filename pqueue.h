#ifndef PQUEUE_H
#define PQUEUE_H

#include "event.h"

typedef Event* ItemT;
typedef double PriorityT;
typedef void (*ItemPrinterT)(ItemT item);
typedef void (*PriorityPrinterT)(PriorityT priority);

struct PQCell;
struct PriorityQueue {
    PQCell* head;
    PriorityQueue() {
      head = NULL;
    }
};

void ShowPriorityQueue(const PriorityQueue& q, ItemPrinterT printItem,
                       PriorityPrinterT printPriority);

bool isEmpty(const PriorityQueue& q);
void insert(PriorityQueue& q, ItemT i, PriorityT p);
void remove(PriorityQueue& q, ItemT& i, PriorityT& p);

#endif //PQUEUE_H