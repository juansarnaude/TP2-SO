#include <defs.h>
#include <memoryManager.h>
#include <queue.h>
#include <scheduler.h>
#include <semaphore.h>

typedef struct
{
    char data[PIPESIZE];
    unsigned int openR;
    unsigned int openW;
    sem_t canRead;
    unsigned int indexR;
    unsigned int indexW;
    BlockedQueueADT queueW;
    BlockedQueueADT queueR;
} Pipe;

typedef struct pipeNode
{
    Pipe *pipe;
    struct pipeNode *next;
    struct pipeNode *previous;
} pipeNode;

typedef pipeNode *pipeList;

pipeList pipesList = NULL;

Pipe *pipeOpen()
{
    Pipe *newPipe = (Pipe *)memoryManagerAlloc(sizeof(Pipe));
    newPipe->openR = 0;
    newPipe->openW = 1;
    newPipe->queueR = newQueue();
    newPipe->queueW = newQueue();

    pipeNode *newPipeNode = (pipeNode *)memoryManagerAlloc(sizeof(pipeNode));
    newPipeNode->pipe = newPipe;
    newPipeNode->next = pipesList;
    newPipeNode->previous = NULL;
    pipesList = newPipeNode;
    return newPipe;
}

// TODO tengo que ver como liberar las queues
int pipeClose(Pipe *pipe)
{
    pipeNode *current = pipesList;
    while (current != NULL && current->pipe != pipe)
    {
        current = current->next;
    }
    if (current == NULL)
        return 0;

    freeQueue(pipe->queueR);
    freeQueue(pipe->queueR);
    if (current->next != NULL)
        current->next->previous = current->previous;
    if (current->previous != NULL)
        current->previous->next = current->next;
    sem_close(pipe->openR);
    sem_close(pipe->openW);
    memory_manager_free(current);
    memory_manager_free(pipe);
    return 1;
}

int pipeRead(Pipe *pipe, char *msg, int size)
{
    pid_t pid = getCurrentPid();
    enqueuePid(pipe->queueR, pid);
    int i = 0;
    while (pipe->indexR < pipe->indexW && i < size)
    {
        msg[i] = pipe->data[pipe->indexR];
        pipe->indexR++;
        i++;
    }
    return msg;
}

int pipeWrite(Pipe *pipe, char *data, int size)
{
    for (int i = 0; i < size; i++, pipe->indexW++)
    {
        pipe->data[pipe->indexW] = data[i];
    }
}
