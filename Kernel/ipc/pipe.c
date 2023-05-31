#include <defs.h>
typedef struct
{
    char data[PIPESIZE];
    unsigned int read_offset;
    unsigned int write_offset;
    unsigned int open_for_read;
    unsigned int open_for_write;
    BlockedQueueADT blocked_in_read;
    BlockedQueueADT blocked_in_write;

} Pipe;

typedef struct pipeNode
{
    Pipe *p;
    struct pipeNode *next;
} PipeNode;

typedef PipeNode *PipeList;

PipeList pipes = NULL;

