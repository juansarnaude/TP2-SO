#include <unistd.h>
#include <memoryManager.h>
#define START_ADDRESS 0xF00000
#define MEMORY_MANAGMENT_NAME 'Heap'

typedef struct blockCDT
{
    size_t size;           // Number of bytes allocated
    struct blockCDT *prev; // Pointer to prevoius block
    struct blockCDT *next; // Pointer to next block
} BlockCDT;

typedef BlockCDT *BlockADT;

size_t totalHeapMemory;
size_t usedHeapMemory;
size_t memoryBlockCount;

BlockADT head = NULL;

void createMemory(size_t size)
{
    totalHeapMemory = size;
    usedHeapMemory = sizeof(BlockCDT);
    memoryBlockCount = 1;
    head = START_ADDRESS;
    head->size = 0;
    head->prev = NULL;
    head->next = NULL;
}

void *memoryManagerAlloc(size_t nbytes)
{
    if (head == NULL || nbytes == 0 || (totalHeapMemory - usedHeapMemory < nbytes))
        return NULL;
    BlockADT current = head;

    while ((totalHeapMemory - usedHeapMemory - current->size) < (nbytes + sizeof(BlockCDT)))
    {
        if (current == NULL)
            return NULL;
        current = current->next;
    }
    if (current->size == 0)
    {
        current->size = nbytes;
        usedHeapMemory += nbytes;
        return (void *)current + sizeof(BlockCDT); // Return pointer to next block
    }
    else
    {
        BlockADT nextBlock = (void *)((size_t)current + current->size + sizeof(BlockCDT));
        nextBlock->size = nbytes;
        current->size = totalHeapMemory - usedHeapMemory - nbytes - sizeof(BlockCDT);
        nextBlock->next = current->next;
        nextBlock->prev = current;
        if (current->next != NULL)
            current->next->prev = nextBlock;
        current->next = nextBlock;
        usedHeapMemory += nbytes + sizeof(BlockCDT);
        memoryBlockCount++;
        return (void *)nextBlock + sizeof(BlockCDT); // Return pointer to next block
    }
}

void memory_manager_free(void *ap)
{
    if (ap == NULL)
        return;
    BlockADT current = (void *)((size_t)ap - sizeof(BlockCDT));
    if (current->prev == NULL)
    {
        usedHeapMemory -= current->size;
        current->size = 0;
    }
    else
    {
        BlockADT prevBlock = current->prev;
        prevBlock->next = current->next;
        if (current->next != NULL)
            current->next->prev = prevBlock;
        usedHeapMemory -= current->size + sizeof(BlockCDT);
        memoryBlockCount--;
    }
}

MemoryInfo *mem_info()
{
    MemoryInfo *info = memoryManagerAlloc(sizeof(MemoryInfo));
    if (info == NULL)
    {
        return NULL;
    }
    // info->memoryAlgorithmName = strcpy(MEMORY_MANAGMENT_NAME);
    info->totalMemory = totalHeapMemory;
    info->occupiedMemory = usedHeapMemory;
    info->freeMemory = totalHeapMemory - usedHeapMemory;
    info->blocksUsed = memoryBlockCount;
    return info;
}