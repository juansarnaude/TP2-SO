#ifndef QUEUE_H
#define QUEUE_H

#include <defs.h>
#include <memoryManager.h>

BlockedQueueADT newQueue();
pid_t dequeuePid(BlockedQueueADT queue);
void enqueuePid(BlockedQueueADT queue, pid_t pid);
void freeQueue(BlockedQueueADT queue);

#endif