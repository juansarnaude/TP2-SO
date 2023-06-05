// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <queue.h>
#include <lib.h>
#include <pipe.h>

extern uint64_t loadProcess(uint64_t rip, uint64_t rsp, uint64_t argc, uint64_t argv); // implement on assembler
extern void _int20h();                                                                 // implement int20h con assembler

// tck and ppriorities
#define STACK_SIZE 4096
#define MIN_PRIORITY 1
#define MAX_PRIORITY 9

#define NUMBER_OF_PRIORITIES 9
#define DEFAULT_PRIORITY 4
priority_t priorities[NUMBER_OF_PRIORITIES] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

/* Status */
// typedef enum
// {
//     READY,
//     RUNNING,
//     BLOCKED,
//     KILLED,
// } State;

// typedef const char *StateStrings[];

// StateStrings stateStrings = {
//     "READY",
//     "RUNNING",
//     "BLOCKED",
//     "KILLED",
// };

// // Proccess info, nodes and lists

// typedef struct processInfo
// {
//     uint64_t pid;
//     uint64_t p_pid;
//     uint64_t priority;
//     State state;
// } pInfo;

// typedef struct processNode
// {
//     pInfo info;
//     struct processNode *next;
// } pNode;

// typedef struct processList
// {
//     pNode *first;
//     pNode *last;
//     uint64_t size;
//     uint64_t readyAmount;
// } pList;

// Queues
Queue active = NULL;
Queue expired = NULL;

// Schelduler states
int processAmount = -1;
unsigned int processReadyCount = 0;
pid_t dummyProcessPid = NULL;
char proccessBeingRun = 0;
int readyProcessAmount = 0;

void dummyProcess()
{
    while (1)
    {
        ncPrint("a");
        _hlt();
    }
}

void createScheduler()
{
    char *name = "Kernel Task";
    char *argv[] = {name};
    dummyProcessPid = createProcess((uint64_t)dummyProcess, 0, NULL);
    for (int i = 0; i <= 2; i++)
    {
        active->process.fileDescriptors[i].mode = OPEN;
    }
    // PIPEOUT, PIPEIN
    for (int i = 3; i <= 4; i++)
    {
        active->process.fileDescriptors[i].mode = CLOSED;
    }
    active->process.lastFd = 4;
    active->process.status = BLOCKED;
    // readyProcessAmount--;
    processReadyCount--;
}

PCB *getProcess(pid_t pid)
{
    Node *current = active;
    while (current != NULL)
    {
        if (current->process.pid == pid)
        {
            return &(current->process);
        }
        else
        {
            current = current->next;
        }
    }
    current = expired;
    while (current != NULL)
    {
        if (current->process.pid == pid)
        {
            return &(current->process);
        }
        else
        {
            current = current->next;
        }
    }
    return NULL;
}

uint64_t getCurrentPid()
{
    if (active != NULL)
    {
        return active->process.pid;
    }
    return -1;
}

int blockProcess(pid_t pid)
{
    Node *current = active;
    char found = 0;

    while (!found && current != NULL)
    {
        if (current->process.pid == pid)
        {
            found = 1;
            current->process.status = BLOCKED;
        }
        else
        {
            current = current->next;
        }
    }
    current = expired;
    while (!found && current != NULL)
    {
        if (current->process.pid == pid)
        {
            found = 1;
            current->process.status = BLOCKED;
        }
        else
        {
            current = current->next;
        }
    }
    if (found)
    {
        processReadyCount--;
        _int20h();
        return 0;
    }
    return -1;
}

int unblockProcess(pid_t pid)
{
    Node *current = active;
    char found = 0;

    while (!found && current != NULL)
    {
        if (current->process.pid == pid)
        {
            found = 1;
            current->process.status = READY;
        }
        else
        {
            current = current->next;
        }
    }
    current = expired;
    while (!found && current != NULL)
    {
        if (current->process.pid == pid)
        {
            found = 1;
            current->process.status = READY;
        }
        else
        {
            current = current->next;
        }
    }
    if (found)
    {
        processReadyCount++;
        return 0;
    }
    return -1;
}

char **copy_argv(int argc, char **argv)
{
    char **new_argv = memoryManagerAlloc(sizeof(char *) * argc);
    for (int i = 0; i < argc; i++)
    {
        new_argv[i] = strcpy(argv[i]);
    }
    return new_argv;
}

pid_t createProcess(uint64_t rip, int argc, char *argv[])
{
    Node *newProcess = memoryManagerAlloc(sizeof(Node));
    newProcess->process.pid = processAmount++;
    newProcess->process.priority = DEFAULT_PRIORITY;
    newProcess->process.quantumsLeft = priorities[DEFAULT_PRIORITY];
    newProcess->process.blockedQueue = newQueue();
    newProcess->process.newPriority = -1;
    newProcess->process.status = READY;
    newProcess->process.argc = argc;
    newProcess->process.argv = copy_argv(argc, argv);

    // STDIN, STDOUT, STDERR, PIPEOUT, PIPEIN
    if (active != NULL)
    {
        for (int i = 0; i <= active->process.lastFd; i++)
        {
            newProcess->process.fileDescriptors[i].mode = active->process.fileDescriptors[i].mode;
        }
        newProcess->process.lastFd = active->process.lastFd;
        newProcess->process.pipe = active->process.pipe;
    }

    uint64_t rsp = (uint64_t)memoryManagerAlloc(4 * 1024);
    if (rsp == 0)
    {
        return -1;
    }
    newProcess->process.stackBase = rsp;
    uint64_t newRsp = (uint64_t)loadProcess(rip, rsp + 4 * 1024, newProcess->process.argc, (uint64_t)newProcess->process.argv);
    newProcess->process.rsp = newRsp;

    if (active == NULL)
    {
        newProcess->next = NULL;
        active = newProcess;
    }
    else
    {
        if (expired == NULL)
        {
            newProcess->next = NULL;
            expired = newProcess;
        }
        else
        {
            Node *current = expired;
            Node *previous = NULL;
            // Si el numero del que quiero insertar es mayor que el current entonces tengo que insertarlo despues
            // new_process -> 2 y current -> 1. La 1 es mejor que la 2. El menor numero gana
            while (current->next != NULL && newProcess->process.priority >= current->process.priority)
            {
                previous = current;
                current = current->next;
            }
            if (current->next == NULL && newProcess->process.priority >= current->process.priority)
            {
                newProcess->next = NULL;
                current->next = newProcess;
            }
            else
            {
                newProcess->next = current;
                if (previous != NULL)
                {
                    previous->next = newProcess;
                }
                else
                {
                    expired = newProcess;
                }
            }
        }
    }
    processReadyCount++;
    return newProcess->process.pid;
}

void nextProcess()
{
    Node *current = active;
    Node *previous = NULL;
    while (current != NULL && current->process.status == BLOCKED)
    {
        previous = current;
        current = current->next;
    }
    if (current != NULL)
    {
        if (previous != NULL)
        {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    }
    else
    {
        Node *aux = active;
        active = expired;
        expired = aux;

        current = active;
        previous = NULL;
        while (current != NULL && current->process.status == BLOCKED)
        {
            previous = current;
            current = current->next;
        }
        if (previous != NULL && current != NULL)
        {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    }
}

int prepareDummyForWork(pid_t pid)
{
    Node *current = active;
    Node *previous = NULL;
    while (current != NULL && current->process.pid != pid)
    {
        previous = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (previous != NULL)
        {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    }
    else
    {
        current = expired;
        previous = NULL;
        while (current != NULL && current->process.pid != pid)
        {
            previous = current;
            current = current->next;
        }
        if (current == NULL)
        {
            return -1;
        }
        if (previous == NULL)
        {
            expired = current->next;
        }
        else
        {
            previous->next = current->next;
        }
        current->next = active;
        active = current;
    }
    return 0;
}

uint64_t contextSwitch(uint64_t rsp)
{
    // C1.1 y C1.3 (Todos)
    if (!proccessBeingRun)
    {
        proccessBeingRun = 1;
        // C1.1 o C1.3.1: NO HAY NADA CORRIENDOSE Y TENGO ALGO PARA CORRER
        if //(readyProcessAmount > 0)
            (processReadyCount > 0)
        {
            nextProcess();
        }
        else
        { // C1.3.2 y C1.3.3
            prepareDummyForWork(dummyProcessPid);
        }
        return active->process.rsp;
    }

    Node *currentProcess = active;
    currentProcess->process.rsp = rsp;

    // Si no tengo procesos en ready, es decir, estan todos bloqueados tengo que correr el dummy
    if //(readyProcessAmount == 0)
        (processReadyCount == 0)
    {
        prepareDummyForWork(dummyProcessPid);
        return active->process.rsp;
    }

    if (currentProcess->process.status != BLOCKED && currentProcess->process.quantumsLeft > 0)
    {
        currentProcess->process.quantumsLeft--;
        return rsp;
    }

    // Acomodo el que termino de correr (no me interesa el status) en su lugar en la lista de expirados
    // teniendo en cuenta su prioridad.
    if (currentProcess->process.quantumsLeft == 0)
    {
        if (currentProcess->process.newPriority != -1)
        {
            currentProcess->process.priority = currentProcess->process.newPriority;
            currentProcess->process.newPriority = -1;
        }
        currentProcess->process.quantumsLeft = priorities[currentProcess->process.priority];

        Node *currentExpired = expired;
        Node *previousExpired = NULL;
        while (currentExpired != NULL && currentProcess->process.priority >= currentExpired->process.priority)
        {
            previousExpired = currentExpired;
            currentExpired = currentExpired->next;
        }
        /*
            Debo colocar el current_process en el lugar indicado dentro de los expirados pero teniendo muy en cuenta
            que antes de cambiar el next de este nodo tengo que hacerlo en el active para evitar problemas.
            En cualquiera de ambos casos active tendra que ser igual a active->next porque paso el current_process a expirados.
        */
        active = active->next;
        if (previousExpired == NULL)
        {
            currentProcess->next = expired;
            expired = currentProcess;
        }
        else
        {
            previousExpired->next = currentProcess;
            currentProcess->next = currentExpired;
        }
        if (active == NULL)
        {
            active = expired;
            expired = NULL;
        }
    }
    nextProcess();
    return active->process.rsp;
}

int killProcess(int returnValue, char autokill)
{
    Node *currentProcess = active;

    pid_t blockedPid;
    while ((blockedPid = dequeuePid(currentProcess->process.blockedQueue)) != -1)
    {
        unblockProcess(blockedPid);
    }
    active = currentProcess->next;
    if (currentProcess->process.status != BLOCKED)
    {
        processReadyCount--;
    }
    for (int i = 0; i < currentProcess->process.argc; i++)
    {
        memory_manager_free(currentProcess->process.argv[i]);
    }
    memory_manager_free(currentProcess->process.argv);
    freeQueue(currentProcess->process.blockedQueue);
    memory_manager_free((void *)currentProcess->process.stackBase);
    if (currentProcess->process.pipe != NULL)
    {
        char msg[1] = {EOF};
        pipeWrite(currentProcess->process.pipe, msg, 1);
        //pipeClose(currentProcess->process.pipe);
    }
    memory_manager_free(currentProcess);
    if (autokill)
    {
        proccessBeingRun = 0;
        _int20h();
    }
    return returnValue;
}

int changePriority(pid_t pid, int priorityValue)
{
    if (priorityValue < 0 || priorityValue > 8)
    {
        return -1;
    }
    PCB *process = getProcess(pid);
    if (process == NULL)
    {
        return -1;
    }

    process->newPriority = priorityValue;
    return 0;
}

int yieldProcess()
{
    active->process.quantumsLeft = 0;
    _int20h();
    return 0;
}

processInfo *getProccessesInfo()
{
    processInfo *first = NULL;
    processInfo *current = NULL;
    Queue currentNode = active;
    pid_t firstPid = active->process.pid;

    while (currentNode != NULL)
    {
        if (current != NULL)
        {
            current->next = (processInfo *)memoryManagerAlloc(sizeof(processInfo));
            current = current->next;
        }
        else
        {
            current = (processInfo *)memoryManagerAlloc(sizeof(processInfo));
        }
        current->pid = currentNode->process.pid;
        if (current->pid == firstPid)
        {
            first = current;
        }
        current->priority = currentNode->process.priority;
        current->stackBase = currentNode->process.stackBase;
        current->status = currentNode->process.status;
        currentNode = currentNode->next;
    }
    currentNode = expired;

    while (currentNode != NULL)
    {
        if (current != NULL)
        {
            current->next = (processInfo *)memoryManagerAlloc(sizeof(processInfo));
            current = current->next;
        }
        else
        {
            current = (processInfo *)memoryManagerAlloc(sizeof(processInfo));
        }
        current->pid = currentNode->process.pid;
        current->priority = currentNode->process.priority;
        current->stackBase = currentNode->process.stackBase;
        current->status = currentNode->process.status;
        currentNode = currentNode->next;
    }
    current->next = NULL;
    return first;
}