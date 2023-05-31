/***************************************************
  Defs.h
****************************************************/

#ifndef _defs_
#define _defs_

#include <stdint.h>

#define NULL ((void*)0)

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

/* Registros en memoria en las syscalls */
#define REG_AMOUNT 15
#define RIP_POS REG_AMOUNT
#define RFLAGS_POS (REG_AMOUNT+2)
#define RSP_POS (REG_AMOUNT+3)

#define PIPESIZE  512
#define MAX_FDS   16

typedef int pid_t;

//Blocked Queue
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

//Semaphores
typedef struct {
    char * name;
    uint64_t value;         //it wont be negative, process that try to wait when 0 will be stacked in blockedProcess
    char locked;            //if its locked its 1 if not 0;
    BlockedQueueADT blockedProcesses;
} semaphore;
 
typedef semaphore * sem_t;

/* Pipe */
typedef struct {
    char data[PIPESIZE];
    unsigned int read_offset;
    unsigned int write_offset;
    unsigned int open_for_read;
    unsigned int open_for_write;
    BlockedQueueADT blocked_in_read;
    BlockedQueueADT blocked_in_write;
} Pipe;

typedef struct pipenode {
    Pipe * p;
    struct pipenode * next;
} PipeNode;

typedef PipeNode * PipeList;

typedef struct {
    unsigned int mode;
    Pipe * pipe;
} fd_t;

typedef unsigned int priority_t;
typedef unsigned int status_t;

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

typedef struct node
{
    PCB process;
    struct node *next;
} Node;

typedef Node *Queue;

#endif