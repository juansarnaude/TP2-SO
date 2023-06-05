#include <syscallDispatcher.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphore.h>
#include <pipe.h>
#include <defs.h>

static uint64_t sys_read(unsigned int fd, char *output, uint64_t count);
static void sys_write(unsigned int fd, const char *buffer, uint64_t count);
static pid_t sys_exec(uint64_t program, unsigned int argc, char *argv[]);
static void sys_exit(int return_value, char autokill);
static void sys_time(sysTime_t *s);
static void sys_copymem(uint64_t address, uint8_t *buffer, uint64_t length);
static MemoryInfo *sys_memInfo();
static void *sys_memMalloc(uint64_t size);
static void sys_memFree(uint64_t ap);
static pid_t sys_waitpid(pid_t pid);
static int sys_kill(pid_t pid);
static int sys_block(pid_t pid);
static int sys_unblock(pid_t pid);
static sem_t sys_sem_open(char *name, uint64_t value);
static int sys_sem_close(sem_t sem);
static int sys_sem_post(sem_t sem);
static int sys_sem_wait(sem_t sem);
static int sys_yieldProcess();
static int sys_nice(pid_t pid, int new_priority);
static int sys_pipe(int pipefd[2]);
static int sys_dup2(int fd1, int fd2);
static int sys_open(int fd);
static int sys_close(int fd);
static processInfo *sys_ps();
static int sys_changeProcessStatus(pid_t pid);
static pid_t sys_getCurrentPid();
static int sys_secondsElapsed();
// AGREGAR SYSCALL EXIT QUE ES LLAMADA EN SCHEDULER.ASM

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t *registers)
{
    switch (rax)
    {
    case 0:
        return sys_read((unsigned int)rdi, (char *)rsi, rdx);
        break;
    case 1:
        sys_write((unsigned int)rdi, (char *)rsi, rdx);
        break;
    case 2:
        return (uint64_t)getRegisters((uint64_t *)rdi);
        break;
    case 3:
        return (uint64_t)sys_exec(rdi, (unsigned int)rsi, (char **)rdx);
        break;
    case 4:
        sys_exit((int)rdi, 1);
        break;
    case 5:
        sys_time((sysTime_t *)rdi);
        break;
    case 6:
        sys_copymem(rdi, (uint8_t *)rsi, rdx);
        break;
    case 7:
        return (uint64_t)sys_memInfo();
        break;
    case 8:
        return (uint64_t)sys_memMalloc(rdi);
        break;
    case 9:
        sys_memFree(rdi);
        break;
    case 10:
        return sys_waitpid((pid_t)rdi);
        break;
    case 11:
        return sys_kill((pid_t)rdi);
        break;
    case 12:
        return sys_block((pid_t)rdi);
        break;
    case 13:
        return sys_unblock((pid_t)rdi);
        break;
    case 14:
        return (uint64_t)sys_sem_open((char *)rdi, (uint64_t)rsi);
        break;
    case 15:
        return sys_sem_close((sem_t)rdi);
        break;
    case 16:
        return sys_sem_post((sem_t)rdi);
        break;
    case 17:
        return sys_sem_wait((sem_t)rdi);
        break;
    case 18:
        (uint64_t) sys_yieldProcess();
        break;
    case 19:
        return (uint64_t)sys_nice((pid_t)rdi, (int)rsi);
        break;
    case 20:
        return (uint64_t)sys_pipe((uint64_t)rdi);
        break;
    case 21:
        return (uint64_t)sys_dup2((uint64_t)rdi, (uint64_t)rsi);
        break;
    case 22:
        return sys_open((int)rdi);
        break;
    case 23:
        return sys_close((int)rdi);
        break;
    case 24:
        return (uint64_t)sys_ps();
        break;
    case 25:
        return sys_changeProcessStatus((pid_t)rdi);
        break;
    case 26:
        return sys_getCurrentPid();
        break;
    case 27:
        return sys_secondsElapsed();
        break;
    }
    return 0;
}

static uint64_t sys_read(unsigned int fd, char *output, uint64_t count)
{
    PCB *pcb = getProcess(getCurrentPid());
    if (pcb->lastFd <= fd)
        return 0;

    if (pcb->fileDescriptors[fd].mode == OPEN)
    {
        return readBuffer(output, count);
    }
    if (pcb->fileDescriptors[PIPEOUT].mode == OPEN)
    {
        return pipeRead(pcb->pipe, output, count);
    }
    return 0;
}

static void sys_write(unsigned int fd, const char *buffer, uint64_t count)
{
    PCB *pcb = getProcess(getCurrentPid());
    if (pcb->lastFd < fd)
        return;
    if (pcb->fileDescriptors[fd].mode == OPEN)
    {
        uint64_t i = 0;
        while (i < count)
        {
            switch (fd)
            {
            case STDOUT:
                ncPrintChar(buffer[i]);
                break;
            case STDERR:
                ncPrintCharFormat(buffer[i], ERROR_FORMAT);
                break;
            }
            i++;
        }
    }
    else if (pcb->fileDescriptors[PIPEIN].mode == OPEN)
    {
        pipeWrite(pcb->pipe, buffer, count);
    }
    return;
}

static pid_t sys_exec(uint64_t program, unsigned int argc, char *argv[])
{
    return createProcess(program, argc, argv);
}

static void sys_exit(int return_value, char autokill)
{
    PCB *pcb = getProcess(getCurrentPid());
    unsigned int lastFd = pcb->lastFd;

    for (int i = 0; i < lastFd; i++)
    {
        // sys_close(i);
    }

    killProcess(return_value, autokill);
}

static void sys_time(sysTime_t *s)
{
    s->day = localDay();
    s->month = localMonth();
    s->year = localYear();
    s->hours = localHours();
    s->minutes = getMinutes();
    s->seconds = getSeconds();
}

static void sys_copymem(uint64_t address, uint8_t *buffer, uint64_t length)
{
    memcpy((void *)buffer, (void *)address, length);
}

static MemoryInfo *sys_memInfo()
{
    return mem_info();
}

static void *sys_memMalloc(uint64_t size)
{
    return memoryManagerAlloc(size);
}

static void sys_memFree(uint64_t ap)
{
    memory_manager_free((void *)ap);
}

static pid_t sys_waitpid(pid_t pid)
{
    PCB *processPcb = getProcess(pid);
    if (processPcb == NULL)
    {
        return -1;
    }

    pid_t currentPid = getCurrentPid();
    enqueuePid(processPcb->blockedQueue, currentPid);
    blockProcess(currentPid);

    return pid;
}

static int sys_kill(pid_t pid)
{
    if (pid <= 0)
    {
        return -1;
    }

    int x = prepareDummyForWork(pid);
    if (x == -1)
    {
        return -1;
    }

    sys_exit(0, 0);
    return 0;
}

static int sys_block(pid_t pid)
{
    if (pid <= 0)
    {
        return -1;
    }
    return blockProcess(pid);
}

static int sys_unblock(pid_t pid)
{
    if (pid <= 0)
    {
        return -1;
    }
    return unblockProcess(pid);
}

static sem_t sys_sem_open(char *name, uint64_t value)
{
    return sem_open(name, value);
}
static int sys_sem_close(sem_t sem)
{
    return sem_close(sem);
}
static int sys_sem_post(sem_t sem)
{
    return sem_post(sem);
}
static int sys_sem_wait(sem_t sem)
{
    return sem_wait(sem);
}

static int sys_yieldProcess()
{
    return yieldProcess();
}

static int sys_nice(pid_t pid, int newPriority)
{
    if (pid <= 0)
    {
        return -1;
    }

    return changePriority(pid, newPriority);
}

static int sys_pipe(int pipefd[2])
{
    PCB *pcb = getProcess(getCurrentPid());
    pcb->fileDescriptors[PIPEIN].mode = OPEN;
    pcb->fileDescriptors[PIPEOUT].mode = OPEN;
    pcb->pipe = pipeOpen();
    pipefd[0] = PIPEIN;
    pipefd[1] = PIPEOUT;
    return 0;
}

static int sys_dup2(int fd1, int fd2)
{
    PCB *pcb = getProcess(getCurrentPid());
    if (fd1 > pcb->lastFd || fd2 > pcb->lastFd || pcb->fileDescriptors[fd2].mode == CLOSED)
        return 0;
    pcb->fileDescriptors[fd1] = pcb->fileDescriptors[fd2];
    return 1;
}
static int sys_open(int fd)
{
    PCB *pcb = getProcess(getCurrentPid());
    if (pcb->lastFd < fd)
        return 0;
    pcb->fileDescriptors[fd].mode = OPEN;
    return 1;
}
static int sys_close(int fd)
{
    PCB *pcb = getProcess(getCurrentPid());
    if (pcb->lastFd < fd)
        return 0;
    pcb->fileDescriptors[fd].mode = CLOSED;
    return 1;
}

static processInfo *sys_ps()
{
    return getProccessesInfo();
}

// Returns READY if unblocked, BLOCKED if blocked, -1 if failed
static int sys_changeProcessStatus(pid_t pid)
{
    PCB *process = getProcess(pid);
    if (process == NULL)
    {
        return -1;
    }
    if (process->status == READY)
    {
        sys_block(pid);
        return BLOCKED;
    }
    else
    {
        sys_unblock(pid);
        return READY;
    }
}

static pid_t sys_getCurrentPid()
{
    return getCurrentPid();
}

static int sys_secondsElapsed()
{
    return seconds_elapsed();
}