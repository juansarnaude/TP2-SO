typedef struct
{
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

typedef struct memoryInfo
{
    char *memoryAlgorithmName;
    uint64_t freeMemory;
    uint64_t occupiedMemory;
    uint64_t totalMemory;
    unsigned int blocksUsed;
} MemoryInfo;

typedef void (*command)(int, char **);
