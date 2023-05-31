#include <semaphore.h>
#include <memoryManager.h>

typedef struct semNode{
    semaphore sem;
    struct semNode * next;
    struct semNode * previous;
} semNode;

typedef semNode * semList;

semList semaphoreList = NULL;

semNode * findSemaphore(char * name){
    semNode * current = semaphoreList;
    while (current != NULL){
        if( strcmp(current->sem.name, name) == 0){
            return current;
        }
    }
    return NULL;
}

// Creates semaphore
// If same with name already exists already exist, doesnt change value
sem_t sem_open(char * name, uint64_t value){
    semNode * semAux = findSemaphore(name);
    if(semAux != NULL){
        return &(semAux->sem);
    }
    semAux = (semNode *) memoryManagerAlloc(sizeof(semNode));
    strcpy(semAux->sem.name, name);
    semAux->sem.value = value;
    semAux->sem.locked = 0; 
    semAux->next = semaphoreList;
    semAux->previous = NULL;
    semaphoreList = semAux;
    return &(semAux->sem);
}

// Removes a semaphore
// 1 if it could remove it, -1 if it failed
int sem_close(sem_t sem){
   semNode * semAux = findSemaphore(sem->name);
    if(semAux == NULL){
        return -1;
    }

    if(semAux->previous != NULL){
       semAux->previous->next = semAux->next; 
    }
    if(semAux->next != NULL){
        semAux->next->previous = semAux->previous;
    }
    
    memory_manager_free(semAux->sem.name);
    memory_manager_free(semAux);
    return 1;
} 

// Adds 1 to the value of a semaphore, if it was blocked (==0) it unblocks
// Returns 1 on success, -1 on error
int sem_post(sem_t sem){
    semNode * semAux = findSemaphore(sem->name);

    if(semAux == NULL){
        return -1;
    }

    
}