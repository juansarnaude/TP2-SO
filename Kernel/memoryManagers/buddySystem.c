#ifdef BUDDY_MM

#include <memoryManager.h>
#define START_ADDRESS 0xF00000
#define MIN_BUDDY_SIZE (8 * 1024)
#define MEMORY_MANAGEMENT_NAME "buddy"
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
size_t buddyMaxCount;
size_t buddyTreeHeight = 0; //Total Height of the Tree

buddyADT table;

//Children status will have number of children, 0 if it is one that cna be liberated and 
//-1 if is son of one that can be liberated

// size MUST be equal to 2MB^k with k>1
void createMemory(size_t size)
{
    size_t finalAddress = START_ADDRESS + size;
    totalMemory = size;

    buddyMaxCount = size / MIN_BUDDY_SIZE;

    size_t auxAmount = MIN_BUDDY_SIZE;
    while(auxAmount != size){
        auxAmount=auxAmount*2;
        buddyTreeHeight++;
    }

    table = START_ADDRESS;
    for (int i = 0; i < buddyMaxCount; i++)
    {
       table[i].occupied = FREE;
       table[i].childrenStatus = FREE;
    }
    memoryManagerAlloc(buddyMaxCount * 2 * sizeof(buddyCDT));
}

size_t firstIndexInHeight(size_t height)
{
    return (POWER_OF_2(height)) - 1;
}

size_t findIndexInHeight(size_t height, size_t index)
{
    return (index - firstIndexInHeight(height));
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
    //Finds the position
    for (; index < maxHeightIndex; index++)
    {
        if (table[index].occupied == FREE)
        {
            return index;
        }
    }    
    return -1;
}

void setDescendants(size_t index, int value){
    if(value==-1){
        table[index].occupied = OCCUPIED;
    } else{
        table[index].occupied = FREE;
    }
    table[index].childrenStatus = value;
    if(index < firstIndexInHeight(buddyTreeHeight+1)){
        setDescendants(LEFT_SON(index), value);
        setDescendants(RIGHT_SON(index), value);
    }
}

void *memoryManagerAlloc(size_t nbytes)
{
    size_t height = 0;
    size_t index = findBuddyposition(nbytes);
    if (index == -1)
    {
        // ERROR
        return -1;
    }
    if (index != 0)
    {
        //Marks parents as occupied
        size_t auxIndex = PARENT(index);
        while (auxIndex > 0)
        {
            table[auxIndex].occupied = OCCUPIED;
            table[auxIndex].childrenStatus += 1;
            auxIndex = PARENT(auxIndex);
            height++;
        }

        //Marks descendants as occupied
        if(index < firstIndexInHeight(buddyTreeHeight)){
            setDescendants(index, -1);
        }
        table[index].occupied = OCCUPIED;
        table[index].childrenStatus = 0;
        table[auxIndex].occupied = OCCUPIED;
        table[auxIndex].childrenStatus += 1;
    }
    buddyCount++;
    usedMemory += MIN_BUDDY_SIZE * POWER_OF_2(buddyTreeHeight - height);
    return START_ADDRESS + findIndexInHeight(height, index) * totalMemory / POWER_OF_2(height);
}

// Must give the start of the memory used
void memory_manager_free(void *ap)
{
    if (ap == NULL)
    {
        return;
    }
    size_t position = (size_t)ap;
    size_t a = buddyTreeHeight;
    size_t indexInMaxHeight = (position - START_ADDRESS) / MIN_BUDDY_SIZE;
    size_t index = indexInMaxHeight + firstIndexInHeight(buddyTreeHeight);
    size_t height = buddyTreeHeight;
    size_t found = 0;

    while (index >= 0 && found == 0)
    {
        if (table[index].occupied == OCCUPIED && table[index].childrenStatus == 0)
        {
            found = 1;
            table[index].occupied = FREE;
        }
        else
        {
            height++;
            index = PARENT(index);
            //Agregar qué pasa si no lo encuentra
        }
    }

    index = PARENT(index);

    while (index > 0)
    {
        table[index].childrenStatus -= 1;
        if (table[index].childrenStatus <= 0)
        {
            table[index].occupied = FREE;
        }
        index = PARENT(index);
    }

    table[index].childrenStatus -= 1;
    if (table[index].childrenStatus <= 0)
    {
        table[index].occupied = FREE;
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
    // info->memoryAlgorithmName = strcpy(MEMORY_MANAGEMENT_NAME);
    info->totalMemory = totalMemory;
    info->occupiedMemory = usedMemory;
    info->freeMemory = totalMemory - usedMemory;
    info->blocksUsed = buddyCount;
    return info;
}

#endif