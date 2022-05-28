#include <syslib.h>

unsigned int strlen(const char *s){
    unsigned int count = 0;
    while(*s!='\0')
    {
        count++;
        s++;
    }
    return count;
}

unsigned int scanf(char *s,unsigned int size){
    return sys_read(0,s,size);
}

char getChar(){
    char c;
    sys_read(0,&c,1);
    return c;
}

void putChar(char c){
    sys_write(1,&c,1);
}

void puts(const char* s){
    while(*s != '\0'){
        putChar(*s);
        s++;
    }
}