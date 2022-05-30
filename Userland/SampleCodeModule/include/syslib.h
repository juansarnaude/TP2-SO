#ifndef SYSLIB_H
#define SYSLIB_H

#define NULL (void*)0;

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define EOF -1

int sys_write(int fd, void *buffer, int size);
int sys_read(int fd,void* buffer,int size);
void sys_time();

unsigned int strlen(const char *s);
int puts(const char* s);
int putChar(char);
char getChar();
char * gets(char * s);
int strcmpBrazil (const char *p1, const char *p2);
int strcmp (const char *p1, const char *p2);
void getTime();

#endif