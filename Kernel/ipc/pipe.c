#include <defs.h>
#include <memoryManager.h>
#include <queue.h>
#include <scheduler.h>

pipeList pipesList = NULL;

Pipe *pipeOpen()
{
    Pipe *newPipe = (Pipe *)memoryManagerAlloc(sizeof(Pipe));
    newPipe->openR = 1;
    newPipe->openW = 1;
    newPipe->queueWriteBlocked = newQueue();
    newPipe->queueReadBlocked = newQueue();

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
    if (current == NULL){
        return 0;
    }

    freeQueue(pipe->queueReadBlocked);
    freeQueue(pipe->queueWriteBlocked);
    if (current->next != NULL){
        current->next->previous = current->previous;
    }  
    if (current->previous != NULL){
        current->previous->next = current->next;
    }   
    memory_manager_free(current);
    memory_manager_free(pipe);
    return 1;
}

//Returns the amount of chars read or -1 if it failed
int pipeRead(Pipe *pipe, char *msg, int size)
{
    if(pipe->openR == 0){
        return -1;
    }

    int i=0;
    pid_t currentPid;
    while (i < size)
    {
        if(pipe->indexR == pipe->indexW){
            currentPid = getCurrentPid();
            enqueuePid(pipe->queueReadBlocked, currentPid);
            blockProcess(currentPid);
        }        
        msg[i] = pipe->data[pipe->indexR %PIPESIZE];
        pipe->indexR ++;
        i++;
        while((currentPid = dequeuePid(pipe->queueWriteBlocked)) != -1){
            unblockProcess(currentPid);
        }        
    }

    return i;
}

//Returns the amount of chars writen or -1 if it failed
int pipeWrite(Pipe *pipe, char *msg, int size)
{
    if(pipe->openW == 0){
        return -1;
    }

    int i=0;
    pid_t currentPid;
    while(i < size){        
        if(pipe->indexW == pipe->indexR + PIPESIZE ){
            currentPid = getCurrentPid();
            enqueuePid(pipe->queueWriteBlocked, currentPid);
            blockProcess(currentPid);
        }
        pipe->data[pipe->indexW % PIPESIZE] = msg[i];
        pipe->indexW ++;
        i++;
        while((currentPid = dequeuePid(pipe->queueReadBlocked)) != -1){
            unblockProcess(currentPid);
        }
    }
    return 1;
}
