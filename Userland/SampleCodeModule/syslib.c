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
    int i=0;
    while(i < size){
        char c =getChar();
        if('\n' == c){
            return i;
        }
        putChar(c);
        i++;
    }
    return i;
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

//https://code.woboq.org/userspace/glibc/string/strcmp.c.html
int strcmp (const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}