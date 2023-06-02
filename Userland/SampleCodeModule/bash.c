#include <bash.h>
#include <syscall.h>
#include <syslib.h>
#include <processes.h>

#define MAX_SIZE_CMD 32
static char buffer[32];

typedef void (*ptr)();
typedef ptr (*pm)();

void help();
int readInput();
void unknownCommand();
void pipeManager();


command command_parser(char * name);
pm commandLine(char *buffer);

command bck_fun = NULL;
int bck_argc = -1;
char ** bck_argv = NULL;

void bash()
{
    int i = 0;
    while (i != -1)
    {
        puts("McWhigginOS:$ ");
        i = readInput();
        putChar('\n');
    }
}

int readInput()
{
    int sizeRead = gets(buffer);
    int part_count;
    char ** parts = strtok(buffer, ' ', &part_count);
    if (strcmp(buffer, "exit") == 0)
    {
        puts("\nGoodbye\n");
        return -1;
    } 
    else if (charBelongs(buffer, '|'))
    {
        putChar('\n');
    }
    else
    {
        command fun = command_parser(parts[0]);
        if (fun == NULL)
        {
            unknownCommand(parts[0]);
        }else{
            pid_t pid = sys_exec((uint64_t) fun, part_count, parts);
            sys_waitpid(pid);
        }
    }
    // etc, para los distintos comandos a implementar
    for (int i = 0; i < part_count; i++) {
        sys_memFree((uint64_t) parts[i]);
    }
    sys_memFree((uint64_t) parts);
    return sizeRead;
}

void unknownCommand()
{
    puts("\nUnknown command: ");
    puts(buffer);
    putChar('\n');
}

command command_parser(char *buffer)
{
    if (strcmp(buffer, "time") == 0)
    {
        putChar('\n');
        return (command)getTime;
    }
    else if (strcmp(buffer, "prime") == 0)
    {
        putChar('\n');
        return (command)printPrime;
    }
    else if (strcmp(buffer, "fibonacci") == 0)
    {
        putChar('\n');
        return (command)fibonacciNumbs;
    }
    else if (strcmp(buffer, "inforeg") == 0)
    {
        putChar('\n');
        return (command)inforeg;
    }
    else if (strcmp(buffer, "dividebyzero") == 0)
    {
        putChar('\n');
        return (command)excepDivZero;
    }
    else if (strcmp(buffer, "help") == 0)
    {
        putChar('\n');
        return (command)help;
    }
    else if (strcmp(buffer, "invalidopcode") == 0)
    {
        putChar('\n');
        return (command)excepInvalidOpcode;
    }
    else if (containsString(buffer, "printmem") >= 0)
    {
        putChar('\n');
        savePrintMemParams(buffer);
        return (command)printmem;
    }
    else if ((strcmp(buffer, "inforeg")) == 0)
    {
        putChar('\n');
        return (command)inforeg;
    }
    else if ((containsString(buffer, "test_mm")) >= 0)
    {
        putChar('\n');
        return (command)test_mm;
    }
    else if ((containsString(buffer, "test_processes")) >= 0)
    {
        putChar('\n');
        return (command)test_processes;
    }
    else if ((containsString(buffer, "test_priority")) >= 0)
    {
        putChar('\n');
        return (command)test_priority;
    }
    else if ((containsString(buffer, "test_sync")) >= 0)
    {
        putChar('\n');
        return (command)test_sync;
    }
    else if((containsString(buffer, "ps")) >= 0){
        putChar('\n');
        return (command)getProcessesInfo; 
    }
    else if(containsString(buffer, "loop") >= 0 ){
        putChar('\n');
        return (command)loopProcess; 
    }
    else if(containsString(buffer, "kill") >= 0 ){
        putChar('\n');
        return (command)killProcess; 
    }
    else if(containsString(buffer, "nice") >= 0 ){
        putChar('\n');
        return (command)niceProcess; 
    }
    else if(containsString(buffer, "block") >= 0 ){
        putChar('\n');
        return (command)blockProcess; 
    }
    return NULL;
}

void help(int argc, char * argv[])
{
    const char *helpstring =
        "help                 Provides help information for commands.\n"
        "dividebyzero         Command to verify the operation of the exception routine\n"
        "                     \"Divide by zero\"\n"
        "invalidopcode        Command to verify the operation of the exception routine\n"
        "                     \"Invalid Opcode\"\n"
        "inforeg              CTRL+C takes a screenshot of registers, inforeg prints on\n"
        "                     screen the value of all registers screenshoted.\n"
        "printmem             Receives as argument a pointer and performs a memory dump\n"
        "                     of 32 bytes from the address received as an argument.\n"
        "                     Format for address has to start with 0x and be followed up\n"
        "                     with the actual address written in hex.\n"
        "time                 Command to display the system day and time.\n"
        "prime                Dispalys prime numbers starting from 1.\n"
        "fibonacci            Dispalys fibonacci series numbers.\n"
        "COMMAND1|COMMAND2    The \"|\" operand allows the execution of multiple commands\n"
        "                     at the same time. CTRL+L and CTRL+R stops and resumes left\n"
        "                     and right windows. CTRL+E stops both windows and returns\n"
        "                     to console.\n";

    puts(helpstring);
}