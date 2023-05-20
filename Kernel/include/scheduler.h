#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <timerDriver.h>
#include <defs.h>

typedef unsigned int pid_t;
typedef unsigned int priority_t;
typedef unsigned int status_t;

typedef struct node
{
    PCB process;
    struct node *next;
} Node;

typedef Node *Queue;

typedef struct blockednode
{
    pid_t pid;
    struct blockednode *next;
} BlockedNode;

typedef struct
{
    BlockedNode *first;
    BlockedNode *last;
    unsigned int qty;
} BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

typedef struct
{
    pid_t pid;
    priority_t priority;
    int newPriority;
    status_t status;
    unsigned int quantumsLeft;
    uint64_t rsp;
    uint64_t stackBase;
    BlockedQueueADT blockedQueue;
    fd_t fileDescriptors[MAX_FDS];
    unsigned int lastFd;
    unsigned int argc;
    char **argv;
} PCB;

#endif // SCHEDULER_H