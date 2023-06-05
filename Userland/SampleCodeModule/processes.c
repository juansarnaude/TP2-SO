#include <syslib.h>
#include <processes.h>

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
        "fibonacci            Dispalys fibonacci series numbers.\n"
        "COMMAND1|COMMAND2    The \"|\" operand allows the output of the first command\n"
        "                     to be the input of the second command. CTRL+D sends an EOF.\n"
        "test_mm <max-mem>    Tests memory manager with <max-mem> bytes.\n"
        "test_priority        Tests priority changes of processes.\n"
        "test_processes <max-proc>    Tests process creation with <max-proc>.\n"
        "test_sync <max-proc> <sem-flag>    Tests synchronization of processes.\n";

    puts(helpstring);
}


void fibonacciNumbs(int argc, char *argv[])
{
    char num[30];
    int t1 = 0, t2 = 1;
    long nextTerm = t1 + t2;
    puts("Fibonacci Series: 0, 1, ");
    while (1)
    {
        itoa(nextTerm, num);
        puts(num);
        puts(",");
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
        if (nextTerm < 0)
        { // por si se pasa del max integer
            puts("\b");
            return;
        }
        puts("\n");
    }
}

void getProcessesInfo(int argc, char *argv[])
{
    processInfo *current = sys_ps();
    char *toPrint;

    while (current != NULL)
    {
        fprintf(STDOUT, "PID: %d\n", current->pid);
        fprintf(STDOUT, "Priority: %d\n", current->priority);
        fprintf(STDOUT, "Stack Base: 0x%x\n", current->stackBase);
        fprintf(STDOUT, "Status: %s\n\n", (current->status) ? "BLOCKED" : "READY");
        sys_memFree(current);
        current = current->next;
    }
}

void loopProcess(int argc, char *argv[])
{
    int secs = 1;
    pid_t currentPid = sys_getCurrentPid();
    while (1)
    {
        wait(secs);
        fprintf(STDOUT, "McWhiggin sends greetings to process with PID: %d\n", (int)currentPid);
    }
}

void killProcess(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts("kill should only receive 1 argument");
        return;
    }
    if (sys_kill(atoi(argv[1])) == 0)
    {
        fprintf(STDOUT, "Killed PID: %d\n", atoi(argv[1]));
    }
    else
    {
        fprintf(STDOUT, "Failed to kill PID: %d\n", atoi(argv[1]));
    }
}

void niceProcess(int argc, char *argv[])
{
    if (argc != 3)
    {
        puts("nice should only receive 2 argument");
        return;
    }
    int a1 = atoi(argv[1]);
    int a2 = atoi(argv[2]);
    if (sys_nice((pid_t)a1, a2))
    {
        fprintf(STDOUT, "Changed priority of PID: %d , to %d\n", a1, a2);
    }
    else
    {
        puts("Failed to chang priority\n");
    }
}

void blockProcess(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts("block should only receive 1 arguments");
        return;
    }
    int a = atoi(argv[1]);
    int retValue = sys_changeProcessStatus(a);

    if (retValue == READY)
    {
        fprintf(STDOUT, "PID: %d unblocked\n", a);
    }
    else if (retValue == BLOCKED)
    {
        fprintf(STDOUT, "PID: %d blocked\n", a);
    }
    else
    {
        puts("Failed to block/unblock the process\n");
    }
}

void cat(int argc, char *argv[])
{
    int i = 0, c = getChar();
    while (c != EOF)
    {
        if (c == '\b' && i > 0)
        {
            i--;
            putChar(c);
        }
        else if (c == '\b' && i == 0)
        {
            // no hago nada
        }
        else
        {
            i++;
            putChar(c);
        }
        c = getChar();
    }
}

void wc(int argc, char *argv[])
{
    int c;
    int lines = 0;
    while ((c = getChar()) != EOF)
    {
        if (c == '\n')
        {
            lines++;
        }
    }
    fprintf(STDOUT, "%d lines detected\n", lines);
}

void filter(int argc, char *argv[])
{

    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    char capitalVowels[] = {'A', 'E', 'I', 'O', 'U'};
    int c;
    while ((c = getChar()) != EOF)
    {
        for (int k = 0; k < 5; k++)
        {
            if (c == vowels[k])
            {
                fprintf(STDOUT, "%c", vowels[k]);
            }
            if (c == capitalVowels[k])
            {
                fprintf(STDOUT, "%c", capitalVowels[k]);
            }
        }
    }
}

void getTime(int argc, char *argv[])
{
    sysTime_t time;
    char buffer[64];
    sys_time(&time);

    putChar('\n');
    uintToBase(time.hours, buffer, 10);
    puts(buffer);
    putChar(':');
    uintToBase(time.minutes, buffer, 10);
    puts(buffer);
    putChar(':');
    uintToBase(time.seconds, buffer, 10);
    puts(buffer);
    putChar('\n');

    uintToBase(time.day, buffer, 10);
    puts(buffer);
    putChar('/');
    uintToBase(time.month, buffer, 10);
    puts(buffer);
    putChar('/');
    uintToBase(time.year + 2000, buffer, 10);
    puts(buffer);
    putChar('\n');
}