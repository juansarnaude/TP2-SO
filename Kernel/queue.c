#include <queue.h>

// Esto es lo que usa un proceso para determinar los procesos bloqueados por el mismo
// Ejemplo: lo usaria el semaforo para saber quienes estan bloqueados esperando al semaforo
BlockedQueueADT newQueue() {
    BlockedQueueADT queue = memoryManagerAlloc(sizeof(BlockedQueueCDT));
    if(queue == NULL) {
        return NULL;
    }
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

pid_t dequeuePid(BlockedQueueADT queue) {
    if(queue == NULL) {
        return -1;
    }
    BlockedNode * first = queue->first;
    if(first == NULL) {
        return -1;
    }
    queue->first = first->next;
    pid_t ans = first->pid;
    memory_manager_free(first);
    return ans;
}

void enqueuePid(BlockedQueueADT queue, pid_t pid) {
    if(queue == NULL) {
        return -1;
    }
    BlockedNode * newNode = memory_manager_alloc(sizeof(BlockedNode));
    newNode->pid = pid;
    newNode->next = NULL;
    if(queue->first == NULL) {
        queue->first = newNode;
        queue->last = newNode;
    } else {
        queue->last->next = newNode;
        queue->last = newNode;
    }
}

void freeQueue(BlockedQueueADT queue) {
    if (queue == NULL) {
        return;
    }

    while (queue->first != NULL) {
        BlockedNode * aux = queue->first;
        queue->first = queue->first->next;
        memory_manager_free(aux);
    }
    memory_manager_free(queue);
}