#include <bash.h>
static char buffer[32];

void bash() {
    help();
    int i=0;
    while(i != -1){
        puts("McWhigginOS:$ ");
        i = readInput();
        putChar('\n');
    }
}

int readInput(){
    int sizeRead = gets(buffer);
    if(strcmp(buffer,"exit") == 0){
        puts("\nGoodbye\n");
        return -1;
    }else if(charBelongs(buffer,'|')){
        putChar('\n');
        pipeManager();
    }else{
        pm fun = commandLine(buffer);
        if(fun != NULL){
            (*fun)();
        }
    }
    //etc, para los distintos comandos a implementar
    return sizeRead;
}

void unknownCommand(){
    puts("\nUnknown command: ");
    puts(buffer);
    putChar('\n');
}

ptr commandLine(char* buffer){
    if(strcmp(buffer,"time") == 0){
        putChar('\n');
        return &getTime;
    }else if(strcmp(buffer,"primenumbs") == 0){
        putChar('\n');
        return &printPrime;
    }else if(strcmp(buffer,"fibonaccinumbs") == 0){
        putChar('\n');
        return &fibonacciNumbs;
    }else if(strcmp(buffer,"inforeg") == 0){
        putChar('\n');
        return &inforeg;
    }else if(strcmp(buffer,"dividebyzero") == 0){
        putChar('\n');
        return &excepDivZero;
    }else if(strcmp(buffer,"help") == 0){
        putChar('\n');
        return &help;
    }else if(strcmp(buffer,"invalidopcode") == 0){
        putChar('\n');
        return &excepInvalidOpcode;
    }else if(containsString(buffer,"printmem") >= 0){
        putChar('\n');
        if(checkPrintMemParams(buffer) == 1){
            return &printmem;
        }
    }else if( (strcmp(buffer,"inforeg")) == 0){
        putChar('\n');
        return &inforeg;
    }else{//el comando ingresado no existe.
        unknownCommand();
    }
    return NULL;
}

void pipeManager(){
    char cmd1[11],cmd2[11];
    unsigned int i=0;
    while(buffer[i] != '|' && i < 11){
        cmd1[i] = buffer[i];
        i++;
    }
    if(i == 11){
        unknownCommand(cmd1);
        return;
    }
    cmd1[i] = '\0';
    i++;//como estoy parado en la '|' paso al siguiente
    unsigned int j=0;
    while(buffer[i] != '\0' && j < 11){
        cmd2[j++] = buffer[i++];
    }
    if(j == 11){
        unknownCommand(cmd2);
        return;
    }
    cmd2[j] = '\0';
    pm fun1 = commandLine(cmd1);
    pm fun2 = commandLine(cmd2);
    if(fun1 == NULL || fun2 == NULL){
        return;
    }
    sys_execve(fun1,fun2);
}

void help(){
    const char* helpstring = 
    "help                 Provides help information for commands.\n"
	"dividebyzero         Command to verify the operation of the exception routine\n"
    "                     \"Divide by zero\"\n"
    "invalidopcode        Command to verify the operation of the exception routine\n"
    "                     \"Invalid Opcode\"\n"
	"inforeg              CTRL+C takes a screenshot of registers, inforeg prints on\n"
    "                     screen the value of all registers screenshoted.\n"
	"printmem             Receives as argument a pointer and performs a memory dump\n"
    "                     of 32 bytes from the address received as an argument.\n"
	"time                 Command to display the system day and time.\n"
    "primenumbs           Dispalys prime numbers starting from 1.\n"
    "fibonaccinumbs       Dispalys fibonacci series numbers.\n"
    "COMMAND1 | COMMAND2  The \"|\" operand allows the execution of multiple commands\n"
    "                     at the same time. CTRL+L and CTRL+R stops and resumes left\n"
    "                     and right windows.\n";
    
    puts(helpstring);
}