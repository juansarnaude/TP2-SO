#include <syscallDispatcher.h>

static uint64_t sys_read(unsigned int fd,char* output, uint64_t count);
static void sys_write(unsigned fd,const char* buffer, uint64_t count);
static uint64_t sys_windows(unsigned int windows);
static uint64_t sys_currentWindow(unsigned int window);
static int sys_exec(int (*program1)(), int (*program2)(), uint64_t * registers);
static void sys_exit(int retValue, uint64_t * registers);
static void sys_time();

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t * registers){
    //TODO: Aumentar la cantidad de registros que nos pasan a 6.
    //TODO: Fijarse si las interrupciones son atendidas mientras esta atendiendo a una syscall.
    switch(rax){
        case 0:
            return sys_read((unsigned int)rdi, (char*)rsi,rdx);
            break;
        case 1:
            sys_write((unsigned int)rdi, (char*)rsi,rdx);
            break;
        case 2:
            return getRegisters();
            break;
        case 3:
            return sys_exec((int(*)()) rdi, (int(*)()) rsi, registers);
            break;
        case 4:
            sys_exit(rdi, registers);
            break;
        case 5:
            sys_time();
            break;
    }
    return 0;
}

static uint64_t sys_read(unsigned int fd,char* output, uint64_t count){
    switch (fd)
    {
    case STDIN:
        return readBuffer(output, count);
        break;
    
    default:
        return 0;
    }
}

static void sys_write(unsigned fd,const char* buffer, uint64_t count){
    uint64_t i = 0;
    while (i < count)
    {
        switch(fd){
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

static uint64_t sys_windows(unsigned int windows){
    return ncWindows(windows);
}

static uint64_t sys_currentWindow(unsigned int window){
    return ncCurrentWindow(window);
}

static int sys_exec(int (*program1)(), int (*program2)(), uint64_t * registers){
    loadTasks(program1, program2, registers);
    return 0;
}

static void sys_exit(int retValue, uint64_t * registers){
    exitTask(retValue, registers);
}

static void sys_time(){
    ncPrintTime();
}