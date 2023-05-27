#include <syscall.h>
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uniqueTypes.h>

MemoryInfo *sys_memInfo();
extern void *sys_memMalloc(uint64_t size);
void sys_memFree(void *ap);

#define MAX_BLOCKS 128

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void test_mm(int argc, char *argv[])
{

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

   if (argc != 2)
     return;

  if ((max_memory = satoi(argv[1])) <= 0)
    return;

  while (1)
  {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory)
    {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = sys_memMalloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address)
      {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
        {
          puts("test_mm ERROR\n");
          return;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_memFree(mm_rqs[i].address);
    puts("test_mm WORKS :D\n");
  }
}