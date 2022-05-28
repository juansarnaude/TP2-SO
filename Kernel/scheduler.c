#include <scheduler.h>

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t registers[15]; // Registers: R15, ..., RBX, RAX
    uint64_t rsp, rip, rflags;
    uint8_t window;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */

#define TASK_ARR_SIZE 2

static TASK_CONTEXT tss[TASK_ARR_SIZE];    //Task array
static int amount = 0;          //Amount of tasks currently running
static int current = 0;         //Currently running task

static void * const userlandAddress = (void*)0x600000;
static void * const stepping = (void*)0x100000;


static void saveContext(uint64_t * registers);
static void loadContext(uint64_t * registers);
static void move(int a, int b);



int executeTask(int (*program)()){
    //Adds a task to the task array.
    if (amount < TASK_ARR_SIZE){
        int position = current+1;
        tss[position].rip = (uint64_t)program;
        tss[position].rsp = (uint64_t) userlandAddress + (uint64_t) stepping * amount;
        //Return address for the program.
        *((uint64_t*)tss[position].rsp) = (uint64_t) exitTask;
        tss[position].window = 0;
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

void nextTask(uint64_t * registers){
    if (amount > 1){
        saveContext(registers);
        current = (current+1) % TASK_ARR_SIZE;
        ncCurrentWindow(tss[current].window);
        loadContext(registers);
    } else if (amount == 0) {
        haltcpu();
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
