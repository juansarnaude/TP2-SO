#define MAX_BUFFER  512

#include <stdint.h>
#include <stddef.h>

// Processes
typedef int pid_t;

// Time
typedef struct
{
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

//Memory
typedef struct memoryInfo
{
    char *memoryAlgorithmName;
    uint64_t freeMemory;
    uint64_t occupiedMemory;
    uint64_t totalMemory;
    unsigned int blocksUsed;
} MemoryInfo;

//Semaphores
typedef struct {
    char * name;
    uint64_t value;         //it wont be negative, process that try to wait when 0 will be stacked in blockedProcess
    uint8_t locked;            //if its locked its 1 if not 0;
    BlockedQueueADT blockedProcesses;
} semaphore;
 
typedef semaphore * sem_t;


typedef void (*command)(int, char **);
