#include <bash.h>
static char buffer[32];

void bash() {
    help();
    int i=0;
    while(i != -1){
        i = readInput();
    }
}

int readInput(){
    int sizeRead = gets(buffer);
    if(strcmpBrazil(buffer,"exit") == 0){
        return -1;
    }else if(strcmpBrazil(buffer,"time") == 0){
        getTime();
    }else{//el comando ingresado no existe.
        puts("Unknown command: ");
        puts(buffer);
    }//etc, para los distintos comandos a implementar
    return sizeRead;
}

void help(){
    const char* helpstring = 
    "HELP                 Provides help information for commands.\n"
	"CLEAR                Clears the console.\n"
	"DIVIDEBYZERO         Command to verify the operation of the exception routine\n"
    "                     \"Divide by zero\"\n"
    "INVALIDOPCODE        Command to verify the operation of the exception routine\n"
    "                     \"Invalid Opcode\"\n"
	"INFOREG              Prints on screen the value of all registers.\n"
	"PRINTMEM             Receives as argument a pointer and performs a memory dump\n"
    "                     of 32 bytes from the address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n"
    "PRIMENUMBS           Dispalys prime numbers starting from 1.\n"
    "FIBONACCINUMBS       Dispalys fibonacci series numbers.\n"
    "COMAND1 | COMAND2    The \"|\" operand allows the execution of multimple comands\n"
    "                     at the same time.\n";
    
    puts(helpstring);
}