#ifndef QUEUE_H
#define QUEUE_H

#include <defs.h>
#include <memoryManager.h>


typedef struct blockednode
{
    pid_t pid;
    struct blockednode *next;
} BlockedNode;

typedef struct
{
    BlockedNode *first;
    BlockedNode *last;
} BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

BlockedQueueADT newQueue();
pid_t dequeuePid(BlockedQueueADT queue);
void enqueuePid(BlockedQueueADT queue, pid_t pid);
void freeQueue(BlockedQueueADT queue);

#endif