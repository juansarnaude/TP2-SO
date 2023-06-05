#include <bash.h>
#include <syslib.h>
#include <processes.h>

#define MAX_SIZE_CMD 32
static char buffer[32];

typedef void (*ptr)();
typedef ptr (*pm)();

void help();
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
char *argvW = NULL;
int argcW = 0;
command readFunc = NULL;

char *argvR = NULL;
int argcR = 0;

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

// Change : to |
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
        int i = 0;
        char found = 0;
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
        sys_memFree((uint64_t)parts[i]);
    }
    sys_memFree((uint64_t)parts);
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
    else if (strcmp(buffer, "prime") == 0)
    {
        return (command)printPrime;
    }
    else if (strcmp(buffer, "fibonacci") == 0)
    {
        return (command)fibonacciNumbs;
    }
    else if (strcmp(buffer, "help") == 0)
    {
        return (command)help;
    }
    else if (containsString(buffer, "printmem") >= 0)
    {
        savePrintMemParams(buffer);
        return (command)printmem;
    }
    else if ((strcmp(buffer, "inforeg")) == 0)
    {
        return (command)inforeg;
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
    return NULL;
}

void help(int argc, char *argv[])
{
    const char *helpstring =
        "cat                  Replicates whatever you input to the shell.\n"
        "phylo                Simulates famous synchronization phylosophers problem.\n"
        "ps                   Prints information about current process.\n"
        "loop                 Prints a greeting every certain period of time.\n"
        "filter               Filters input to print only its vowels.\n"
        "block <pid>          Blocks/unblocks process with pid = <pid>.\n"
        "kill <pid>           Kills process with pid = <pid>.\n"
        "nice <pid> <prio>    Changes priority of process with pid = <pid> to <prio>.\n"
        "wc                   Prints the number of newlines from input.\n"
        "help                 Provides help information for commands.\n"
        "time                 Command to display the system day and time.\n"
        "prime                Dispalys prime numbers starting from 1.\n"
        "fibonacci            Dispalys fibonacci series numbers.\n"
        "COMMAND1|COMMAND2    The \"|\" operand allows the output of the first command\n"
        "                     to be the input of the second command. CTRL+D sends an EOF.\n"
        "test_mm <max-mem>    Tests memory manager with <max-mem> bytes.\n"
        "test_priority        Tests priority changes of processes.\n"
        "test_processes <max-proc>    Tests process creation with <max-proc>.\n"
        "test_sync <max-proc> <sem-flag>    Tests synchronization of processes.\n";

    puts(helpstring);
}

void pipeSeparator(char **parts, int part_count, int pipePosition)
{
    command writeFunction = command_parser(parts[0]);
    if (writeFunction == NULL)
    {
        return;
    }
    command readFunction = command_parser(parts[pipePosition + 1]);
    if (readFunction == NULL)
    {
        return;
    }
    pid_t auxPid = sys_getCurrentPid();

    sys_pipe(fds);

    sys_close(fds[1]);
    sys_dup2(fds[0], STDOUT);
    sys_close(STDOUT);
    pid_t pidW = sys_exec((uint64_t)writeFunction, pipePosition, parts);
    sys_waitpid(pidW);

    argcR = part_count - (pipePosition + 1);
    argvR = &parts[pipePosition + 1];
    readFunc = readFunction;

    sys_close(fds[0]);
    sys_dup2(fds[1], STDIN);
    sys_close(STDIN);
    sys_open(STDOUT);
    // pid_t pidR = sys_exec(readFunc, argcR, argvR);
    pid_t pidR = sys_exec((uint64_t)functionRead, 0, NULL);

    sys_waitpid(pidR);

    sys_open(STDIN);
    sys_close(fds[0]);
    sys_close(fds[1]);
}

void functionRead(int argc, char *argv[])
{

    sys_close(fds[0]);
    sys_dup2(fds[1], STDIN);
    sys_close(STDIN);
    sys_open(STDOUT);
    pid_t pidR = sys_exec(readFunc, argcR, argvR);
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
    pid_t pid = sys_exec((uint64_t)backgroundFun, backgroundArgc, backgroundArgv);
}