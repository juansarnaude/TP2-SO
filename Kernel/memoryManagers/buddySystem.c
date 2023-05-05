#include <memoryManager.h>
#define START_ADDRESS 0xF00000
#define MIN_BUDDY_SIZE (8 * 1024 * 1024)

#define FREE 0
#define OCCUPIED 1

typedef struct buddyCDT
{
    size_t occupied;
    size_t childrenStatus;
} buddyCDT;
typedef buddyCDT *buddyADT;

#define LEFT_SON(index) ((index)*2 + 1)
#define RIGHT_SON(index) ((index)*2 + 2)
#define PARENT(index) (((index) + 1) / 2 - 1)
#define POWER_OF_2(x) ((x) ? (1 << (x)) : 1)

size_t totalMemory;
size_t usedMemory;
size_t buddyCount;
size_t buddyTreeHeight = 0;

// size must be equal to 2MB^k with k>1
void createMemory(size_t size)
{
    size_t finalAddress = START_ADDRESS + size;
    totalMemory = size;
    buddyADT = finalAddress;

    buddyCount = size / MIN_BUDDY_SIZE;

    for (int i = 0; i < buddyCount; i++)
    {
        buddyADT[i].occupied = FREE;
        buddyADT[i].childrenStatus = FREE;
    }

    size_t buddyCountAux = buddyCount;
    for (int i = 0; buddyTreeHeight != 0; i++)
    {
        if (buddyCountAux == 1)
        {
            buddyTreeHeight = i;
        }
        buddyCountAux >> 1;
    }
}

size_t firstIndexInHeight(size_t height)
{
    return (1 << (buddyTreeHeight - height)) - 1;
}

size_t findIndexInHeight(size_t height, size_t index)
{
    return index - firstIndexInHeight(height);
}

// Returns the index of the buddy in which it will be positioned. -1 if couldnt find
size_t findBuddyposition(size_t nbytes)
{
    size_t bytesNeeded = MIN_BUDDY_SIZE;
    size_t height = buddyTreeHeight;
    while (bytesNeeded < nbytes)
    {
        bytesNeeded = bytesNeeded * 2;
        height--;
    }

    size_t index = POWER_OF_2(height) - 1;
    size_t maxHeightIndex = POWER_OF_2(height + 1) - 1;
    for (; index < maxHeightIndex; index++)
    {
        if (buddyADT[index].ocuppied == FREE)
        {
            return index;
        }
    }
    return -1;
}

void *memoryManagerAlloc(size_t nbytes)
{
    size_t index = findBuddyPosition(nbytes, height);
    if (index == -1)
    {
        // ERROR
    }
    size_t height = 0;
    if (index != 0)
    {
        size_t auxIndex = index;
        while (auxIndex >= 0)
        {
            buddyADT[auxIndex].occupied = OCCUPIED;
            buddyADT[auxIndex].childrenStatus += 1;
            auxIndex = PARENT(auxIndex);
            height++;
        }
    }

    buddyADT[index].occupied = OCCUPIED;
    buddyCount++;
    usedMemory += MIN_BUDDY_SIZE * POWER_OF_2(buddyTreeHeight - height);
    // Aca esta mandando la direccion de la tabla de buddys y no la direccion de la memoria
    return START_ADDRESS + indexInHeight(height, index) * totalMemory / POWER_OF_2(height);
}

// Must give the start of the memory used
void memory_manager_free(void *ap)
{
    if (ap == NULL)
    {
        return;
    }
    size_t position = (size_t)ap;

    size_t indexInMaxHeight = (position - START_ADDRESS) / MIN_BUDDY_SIZE;
    size_t index = indexInHeight(buddyTreeHeight, indexInMaxHeight);
    size_t height = buddyTreeHeight;
    size_t found = 0;

    while (index >= 0 && found == 0)
    {
        if (buddyADT[index].occupied == OCCUPIED)
        {
            found = 1;
            buddyADT[index].ocuppied = FREE;
        }
        else
        {
            height++;
            index = PARENT(index);
        }
    }

    index = PARENT(index);

    while (index >= 0)
    {
        buddyADT[index].childrenStatus -= 1;
        if (buddyADT[index].childrenStatus <= 0)
        {
            buddyADT[index].occupied = FREE;
        }
    }

    usedMemory -= MIN_BUDDY_SIZE * POWER_OF_2(buddyTreeHeight - height);
    buddyCount--;
}

MemoryInfo *mem_info()
{
    MemoryInfo *info = memoryManagerAlloc(sizeof(MemoryInfo));
    if (info == NULL)
    {
        return NULL;
    }
    // info->memoryAlgorithmName = strcpy(MEMORY_MANAGMENT_NAME);
    info->totalMemory = totalMemory;
    info->occupiedMemory = usedMemory;
    info->freeMemory = totalMemory - usedMemory;
    info->blocksUsed = buddyCount;
    return info;
}