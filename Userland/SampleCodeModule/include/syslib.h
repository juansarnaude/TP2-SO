#ifndef SYSLIB_H
#define SYSLIB_H

void sys_write(int fd, void *buffer, int size);
int sys_read(int fd,void* buffer,int size);
void sys_time();
void bash();
unsigned int strlen(const char *s);
void puts(const char* s);
void putChar(char);
char getChar();
unsigned int scanf(char *s,unsigned int size);
int strcmpBrazil (const char *p1, const char *p2);
int strcmp (const char *p1, const char *p2);
void getTime();
void fibonacciNumbs();

#endif