#include<syslib.h>
static char buffer[32];

void bash() {
    int i=0;
    while(i != -1){
        i = readInput();
    }
}

int readInput(){
    unsigned int sizeRead = scanf(buffer,32);
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