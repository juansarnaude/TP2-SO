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



typedef void (*command)(int, char **);
