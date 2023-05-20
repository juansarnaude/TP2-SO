#include <syscallDispatcher.h>
#include <memoryManager.h>

static uint64_t sys_read(unsigned int fd, char *output, uint64_t count);
static void sys_write(unsigned fd, const char *buffer, uint64_t count);
static int sys_exec(int (*program1)(), int (*program2)(), uint64_t *registers);
static void sys_exit(int retValue, uint64_t *registers);
static void sys_time(time_t *s);
static void sys_copymem(uint64_t address, uint8_t *buffer, uint64_t length);
static MemoryInfo *sys_memInfo();
static void *sys_memMalloc(uint64_t size);
static void sys_memFree(void *ap);

//AGREGAR SYSCALL EXIT QUE ES LLAMADA EN SCHEDULER.ASM

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
        return sys_exec((int (*)())rdi, (int (*)())rsi, registers);
        break;
    case 4:
        sys_exit(rdi, registers);
        break;
    case 5:
        sys_time((time_t *)rdi);
        break;
    case 6:
        sys_copymem(rdi, (uint8_t *)rsi, rdx);
        break;
    case 7:
        return sys_memInfo();
        break;
    case 8:
        return (uint64_t)sys_memMalloc(rdi);
        break;
    case 9:
        sys_memFree(rdi);
        break;
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

static int sys_exec(int (*program1)(), int (*program2)(), uint64_t *registers)
{
    loadTasks(program1, program2, registers);
    return 0;
}

static void sys_exit(int retValue, uint64_t *registers)
{
    exitTask(retValue, registers);
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

static void sys_memFree(void *ap)
{
    memory_manager_free(ap);
}