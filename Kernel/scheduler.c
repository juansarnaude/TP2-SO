#include <scheduler.h>

#define DEFAULT_CURRENT -1
#define TASK_ARR_SIZE 2
#define DEFAULT_FLAGS 0x202 // Interrupts and Intel reserved bits: 1000000010b = 0x202

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t registers[REG_AMOUNT]; // Registers: R15, ..., RBX, RAX
    uint64_t rsp, rip, rflags;
    uint64_t buRIP;
    uint8_t window, active, finished;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */


static TASK_CONTEXT origin;     //Backup del caller.
static TASK_CONTEXT tasks[TASK_ARR_SIZE];    //Task array

//Amount of tasks currently running
//If amount == -1, then tasks where terminated and original context must be reloaded
static int amount = 0;
static int current = DEFAULT_CURRENT;         //Currently running task

static void * const userlandAddress = (void*)0x700000;
static void * const stepping = (void*)0x100000;


static void saveContext(uint64_t * registers);
static void loadContext(uint64_t * registers);
static void saveOrigin(uint64_t * registers);
static void loadOrigin(uint64_t * registers);
static void doNothing();

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
        tasks[i].rflags = DEFAULT_FLAGS;
    }
    amount = 2;

    //Save origin context
    saveOrigin(registers);
    nextTask(registers);
}

void pauseTask(unsigned int taskNum){
    int i = taskNum % TASK_ARR_SIZE;
    if (tasks[i].active == 1){
        tasks[i].buRIP = tasks[i].rip;
        tasks[i].rip = haltcpu;
        tasks[i].active = 0;
    } else {
        tasks[i].rip = tasks[i].buRIP;
        tasks[i].active = 1;
    }
}

void nextTask(uint64_t * registers){
    if (amount == 0)
        return;
    if (amount == -1){
        ncWindows(1);
        loadOrigin(registers);
        return;
    }
    int next = (current+1) % TASK_ARR_SIZE;

    if (tasks[next].finished == 1){
        if (tasks[current].finished == 1){
            ncWindows(1);
            loadContext(registers);
            return;
        } else
            return;
    }

    if (current != DEFAULT_CURRENT && tasks[current].finished != 1)
        saveContext(registers);
    current = next;
    ncCurrentWindow(tasks[current].window);
    loadContext(registers);
}

void exitTask(int retValue, uint64_t * registers){
    if (amount == 0)
        return;
    tasks[current].finished = 1;
    amount--;
    ncNewline();
    ncPrint("Program returned ");
    ncPrintDec(retValue);
    if (amount == 0){
        current = DEFAULT_CURRENT;
        ncWindows(1);
        loadOrigin(registers);
    } else
        nextTask(registers);
}

static void saveContext(uint64_t * registers){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        tasks[current].registers[i] = registers[i];
    }
    tasks[current].rip = registers[RIP_POS];
    tasks[current].rflags = registers[RFLAGS_POS];
    tasks[current].rsp = registers[RSP_POS];
}

static void loadContext(uint64_t * registers){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        registers[i] = tasks[current].registers[i];
    }
    registers[RIP_POS] = tasks[current].rip;
    registers[RFLAGS_POS] = tasks[current].rflags;
    registers[RSP_POS] = tasks[current].rsp;
}

static void saveOrigin(uint64_t * registers){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        origin.registers[i] = registers[i];
    }
    origin.rip = registers[RIP_POS];
    origin.rflags = registers[RFLAGS_POS];
    origin.rsp = registers[RSP_POS];
}

static void loadOrigin(uint64_t * registers){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        registers[i] = origin.registers[i];
    }
    registers[RIP_POS] = origin.rip;
    registers[RFLAGS_POS] = origin.rflags;
    registers[RSP_POS] = origin.rsp;
}

void terminateTasks(){
    for (int i = 0; i < TASK_ARR_SIZE; i++)
    {
        tasks[i].finished = 1;
    } 
    current = DEFAULT_CURRENT;
    amount = -1;
}

void doNothing(){
    while (1)
    {
        _hlt();
    }
    
}
