#include <syscallDispatcher.h>

static uint64_t sys_read(unsigned int fd,char* output, uint64_t count);
static void sys_write(unsigned fd,const char* buffer, uint64_t count);
static uint64_t sys_windows(unsigned int windows);
static uint64_t sys_currentWindow(unsigned int window);
static int sys_exec(int (*function)());
static void sys_exit();
static void sys_time();

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax){
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
             return sys_windows((unsigned int)rdi);//setea la cantidad de windows
            break;
        case 3:
            return sys_currentWindow((unsigned int)rdi);//selecciona la window para printear
        case 11:
            return sys_exec((void (*)())rdi);
            break;
        case 60:
            sys_exit();
        case 100:
            sys_time();
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

#define STRING_SIZE 100
static char string[STRING_SIZE] = {0};

static void sys_write(unsigned fd,const char* buffer, uint64_t count){
    uint64_t i, j;
    for (i = 0, j=0; i < count;i++)
    {
        if (j == STRING_SIZE){
            switch (fd)
            {
            case STDOUT:
                ncPrint(string);
                break;
    
            case STDERR:
                ncPrintFormat(string, ERROR_FORMAT);
                break;
            default:
                return;
            }
            j=0;
        }
        string[j++] = buffer[i];
        
    }
    string[j] = '\0';
    switch (fd)
    {
    case STDOUT:
        ncPrint(string);
        break;

    case STDERR:
        ncPrintFormat(string, ERROR_FORMAT);
        break;
    default:
        return;
    } 
}

static uint64_t sys_windows(unsigned int windows){
    return ncWindows(windows);
}

static uint64_t sys_currentWindow(unsigned int window){
    return ncCurrentWindow(window);
}

static int sys_exec(int (*function)()){    
    return executeTask(function);
}

static void sys_exit(){
    exitTask();
}

static void sys_time(){
    ncPrintTime();
}