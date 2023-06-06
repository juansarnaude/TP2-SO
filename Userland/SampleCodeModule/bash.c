#include <bash.h>
#include <syslib.h>
#include <processes.h>

#define MAX_SIZE_CMD 32
static char buffer[32];

typedef void (*ptr)();
typedef ptr (*pm)();

void sh();
int readInput();
void unknownCommand();
void pipeSeparator(char **parts, int part_count, int pipePosition);
void backgroundManager(char *fun, int part_count, char **parts);
void backgroundFunction(command fun, int argc, char *argv[]);
void functionRead(int argc, char *argv[]);
void functionWrite(int argc, char *argv[]);

command command_parser(char *name);
pm commandLine(char *buffer);

char **backgroundArgv = NULL;
int backgroundArgc = -1;
command backgroundFun = NULL;

int fds[2];
command readFunc = NULL;

char **argvR = NULL;
int argcR = 0;

char **argvW = NULL;
int argcW = 0;
command writeFunc = NULL;

void bash()
{
    sh();
}

void sh()
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
    char **parts = strtok(buffer, ' ', &part_count);
    if (strcmp(buffer, "exit") == 0)
    {
        puts("\nGoodbye\n");
        return -1;
    }
    else if (charBelongs(buffer, '|'))
    {
        int i = 0;
        char found = 0;
        while (!found && i < part_count)
        {
            if (strcmp(parts[i], "|") == 0)
            {
                found = 1;
            }
            else
            {
                i++;
            }
        }
        putChar('\n');
        pipeSeparator(parts, part_count, i);
    }
    else if (charBelongs(buffer, '&'))
    {
        backgroundArgv = parts;
        backgroundArgc = part_count;
        backgroundManager(parts[0], part_count, parts);
    }
    else
    {
        command fun = command_parser(parts[0]);
        if (fun == NULL)
        {
            unknownCommand(parts[0]);
        }
        else
        {
            putChar('\n');
            pid_t pid = sys_exec((uint64_t)fun, part_count, parts);
            sys_waitpid(pid);
        }
    }
    // etc, para los distintos comandos a implementar
    for (int i = 0; i < part_count; i++)
    {
        sys_memFree((void *)parts[i]);
    }
    sys_memFree((void *)parts);
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
        return (command)getTime;
    }
    else if (strcmp(buffer, "fibonacci") == 0)
    {
        return (command)fibonacciNumbs;
    }
    else if (strcmp(buffer, "help") == 0)
    {
        return (command)help;
    }
    else if ((strcmp(buffer, "test_mm")) == 0)
    {
        return (command)test_mm;
    }
    else if ((strcmp(buffer, "test_processes")) == 0)
    {
        return (command)test_processes;
    }
    else if ((strcmp(buffer, "test_priority")) == 0)
    {
        return (command)test_priority;
    }
    else if ((strcmp(buffer, "test_sync")) == 0)
    {
        return (command)test_sync;
    }
    else if ((strcmp(buffer, "ps")) == 0)
    {
        return (command)getProcessesInfo;
    }
    else if (strcmp(buffer, "loop") == 0)
    {
        return (command)loopProcess;
    }
    else if (strcmp(buffer, "kill") == 0)
    {
        return (command)killProcess;
    }
    else if (strcmp(buffer, "nice") == 0)
    {
        return (command)niceProcess;
    }
    else if (strcmp(buffer, "block") == 0)
    {
        return (command)blockProcess;
    }
    else if (strcmp(buffer, "cat") == 0)
    {
        return (command)cat;
    }
    else if (strcmp(buffer, "wc") == 0)
    {
        return (command)wc;
    }
    else if (strcmp(buffer, "filter") == 0)
    {
        return (command)filter;
    }
    else if (strcmp(buffer, "phylo") == 0)
    {
        return (command)phylo;
    }
    else if (strcmp(buffer, "mem") == 0)
    {
        return (command)mem;
    }
    return NULL;
}

void pipeSeparator(char **parts, int part_count, int pipePosition)
{
    command writeFunction = command_parser(parts[0]);
    if (writeFunction == NULL)
    {
        puts("Unkown input function in Pipe");
        putChar('\n');
        return;
    }
    command readFunction = command_parser(parts[pipePosition + 1]);
    if (readFunction == NULL)
    {
        puts("Unkown output function in Pipe");
        putChar('\n');
        return;
    }
    sys_pipe(fds);

    sys_close(fds[1]);
    sys_dup2(fds[0], STDOUT);
    sys_close(STDOUT);
    writeFunc = writeFunction;
    argvW = parts;
    argcW = pipePosition;
    pid_t pidW = sys_exec((uint64_t)functionWrite, 0, NULL);

    argcR = part_count - (pipePosition + 1);
    argvR = &parts[pipePosition + 1];
    readFunc = readFunction;

    sys_close(fds[0]);
    sys_dup2(fds[1], STDIN);
    sys_close(STDIN);
    sys_open(STDOUT);
    pid_t pidR = sys_exec((uint64_t)functionRead, 0, NULL);

    sys_waitpid(pidW);
    sys_waitpid(pidR);

    sys_open(STDIN);
    sys_close(fds[0]);
    sys_close(fds[1]);
}

void functionWrite(int argc, char *argv[])
{
    sys_close(fds[1]);
    sys_dup2(fds[0], STDOUT);
    sys_close(STDOUT);
    pid_t pidW = sys_exec((uint64_t)writeFunc, argcW, argvW);
    sys_waitpid(pidW);
}

void functionRead(int argc, char *argv[])
{

    sys_close(fds[0]);
    sys_dup2(fds[1], STDIN);
    sys_close(STDIN);
    sys_open(STDOUT);
    pid_t pidR = sys_exec((uint64_t)readFunc, argcR, argvR);
    sys_waitpid(pidR);
}

void backgroundManager(char *fun, int part_count, char **parts)
{
    command auxFun = command_parser(fun + 1);
    if (auxFun == NULL)
        return;
    backgroundFun = auxFun;
    pid_t pid = sys_exec((uint64_t)backgroundFunction, 0, NULL);
    sys_waitpid(pid);
}

void backgroundFunction(command fun, int argc, char *argv[])
{
    sys_close(STDIN);
    sys_close(STDOUT);
    sys_exec((uint64_t)backgroundFun, backgroundArgc, backgroundArgv);
}