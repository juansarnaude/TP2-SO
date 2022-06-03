#include <scheduler.h>

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t registers[15]; // Registers: R15, ..., RBX, RAX
    uint64_t rsp, rip, rflags;
    uint8_t window, active, finished;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */

#define TASK_ARR_SIZE 2

static TASK_CONTEXT origin;     //Backup del caller.
static TASK_CONTEXT tasks[TASK_ARR_SIZE];    //Task array
static int amount = 0;          //Amount of tasks currently running
static int current = -1;         //Currently running task

static void * const userlandAddress = (void*)0x700000;
static void * const stepping = (void*)0x100000;


static void saveContext(uint64_t * registers);
static void loadContext(uint64_t * registers);
static void saveOrigin(uint64_t * registers);
static void loadOrigin(uint64_t * registers);

void loadTasks(int (*program1)(), int (*program2)(), uint64_t * registers){
    ncWindows(2);
    tasks[0].rip = (uint64_t) program1;
    tasks[1].rip = (uint64_t) program2;
    for (int i = 0; i < TASK_ARR_SIZE; i++)
    {
        tasks[i].rsp = (uint64_t) userlandAddress + (uint64_t) stepping * i;
        //Return address
        *((uint64_t*)tasks[i].rsp) = (uint64_t) _defaultExit;
        tasks[i].window = i;
        tasks[i].active = 1;
        tasks[i].finished = 0;
        tasks[i].rflags = 0x0000000000000206;
    }
    amount = 2;

    //Save origin context
    saveOrigin(registers);
    nextTask(registers);
}

void pauseTask(unsigned int taskNum){
    tasks[taskNum % TASK_ARR_SIZE].active = ! tasks[taskNum % TASK_ARR_SIZE].active;
}

void nextTask(uint64_t * registers){
    if (amount == 0)
        return;
    int next = (current+1) % TASK_ARR_SIZE;
    if (! tasks[next].active || tasks[next].finished)
        return;
    if (current != -1 && tasks[current].finished != 1)
        saveContext(registers);
    current = next;
    ncCurrentWindow(tasks[current].window);
    loadContext(registers);
}

void exitTask(int retValue, uint64_t * registers){
    tasks[current].finished = 1;
    amount--;
    ncNewline();
    ncPrint("Program returned ");
    ncPrintDec(retValue);
    if (amount == 0){
        ncWindows(1);
        loadOrigin(registers);
    } else
        nextTask(registers);
}

static void saveContext(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        tasks[current].registers[i] = registers[i];
    }
    tasks[current].rip = registers[15];
    tasks[current].rflags = registers[17];
    tasks[current].rsp = registers[18];
}

static void loadContext(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        registers[i] = tasks[current].registers[i];
    }
    registers[15] = tasks[current].rip;
    registers[17] = tasks[current].rflags;
    registers[18] = tasks[current].rsp;
}

static void saveOrigin(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        origin.registers[i] = registers[i];
    }
    origin.rip = registers[15];
    origin.rflags = registers[17];
    origin.rsp = registers[18];
}

static void loadOrigin(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        registers[i] = origin.registers[i];
    }
    registers[15] = origin.rip;
    registers[17] = origin.rflags;
    registers[18] = origin.rsp;
}
