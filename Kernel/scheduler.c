#include <scheduler.h>

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t registers[15]; // Registers: R15, ..., RBX, RAX
    uint64_t rsp, rip, rflags;
    uint8_t window, active;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */

#define TASK_ARR_SIZE 2

static TASK_CONTEXT tss[TASK_ARR_SIZE];    //Task array
static int amount = 0;          //Amount of tasks currently running
static int current = -1;         //Currently running task

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const userlandAddress = (void*)0x600000;
static void * const stepping = (void*)0x100000;


static void saveContext(uint64_t * registers);
static void loadContext(uint64_t * registers);
static void move(int a, int b);



int executeTask(int (*program)()){
    //Adds a task to the task array.
    if (amount < TASK_ARR_SIZE){
        int position = amount;
        tss[position].rip = (uint64_t)program;
        tss[position].rsp = (uint64_t) userlandAddress + (uint64_t) stepping * amount;
        //Return address for the program.
        *((uint64_t*)tss[position].rsp) = (uint64_t) exitTask;
        tss[position].window = 0;
        tss[position].active = 1;
        if (amount){
            tss[position].window = 1;
            ncWindows(2);
        } else {
            ncWindows(1);
        }
        amount++;
        return 1;
    }
    return 0;
}

void pauseTask(unsigned int taskNum){
    tss[taskNum % TASK_ARR_SIZE].active = ! tss[taskNum % TASK_ARR_SIZE].active;
}

void nextTask(uint64_t * registers){
    if (amount > 1){
        int next = (current+1) % TASK_ARR_SIZE;
        if (! tss[next].active)
            return;
        saveContext(registers);
        current = next;
        ncCurrentWindow(tss[current].window);
        loadContext(registers);
    } else if (amount == 0) {
        executeTask(sampleCodeModuleAddress);
        current = 0;
        loadContext(registers);
    }
}

void exitTask(int retValue){
    for (int i = current; i < TASK_ARR_SIZE - 1; i++)
    {
        move(i, i+1);
    }
    amount--;
    ncNewline();
    ncPrint("Program returned ");
    ncPrintDec(retValue);
    // Wait for the next timer tick.
    while (1)
    {
        ;
    }
}

static void saveContext(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        tss[current].registers[i] = registers[i];
    }
    tss[current].rsp = registers[18];
    tss[current].rip = registers[15];
    tss[current].rflags = registers[17];
}

static void loadContext(uint64_t * registers){
    for (int i = 0; i < 15; i++)
    {
        registers[i] = tss[current].registers[i];
    }
    registers[18] = tss[current].rsp;
    registers[15] = tss[current].rip;
    registers[17] = tss[current].rflags;
}

static void move(int a, int b){
    for (int i = 0; i < 15; i++)
    {
        tss[a].registers[i] = tss[b].registers[i];
    }
    tss[a].rsp = tss[b].rsp;
    tss[a].rip = tss[b].rip;
    tss[a].rflags = tss[b].rflags;
    tss[a].window = tss[b].window;
}
