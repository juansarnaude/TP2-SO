#include <syscallDispatcher.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphore.h>

static uint64_t sys_read(unsigned int fd, char *output, uint64_t count);
static void sys_write(unsigned fd, const char *buffer, uint64_t count);
static pid_t sys_exec(uint64_t program, unsigned int argc, char *argv[]);
static void sys_exit(int return_value, char autokill);
static void sys_time(time_t *s);
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
        sys_time((time_t *)rdi);
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
        return (uint64_t)sys_yieldProcess();
        break
    }
    return 0;
}

static uint64_t sys_read(unsigned int fd, char *output, uint64_t count)
{
    switch (fd)
    {
    case STDIN:
        return readBuffer(output, count);
        break;

    default:
        return 0;
    }
}

static void sys_write(unsigned fd, const char *buffer, uint64_t count)
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
        default:
            return;
        }
        i++;
    }
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

static void sys_time(time_t *s)
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
    yieldProcess();
    return;
}