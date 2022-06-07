#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdint.h>

#define NULL (void*)0

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define EOF -1

#include <stdint.h>

int sys_write(int fd, const char *buffer, int size);
int sys_read(int fd,char * buffer,int size);
void sys_time();
int sys_getregs(uint64_t * registers);
void inv_opcode();
void sys_execve(int (*f1)(), int (*f2)());

unsigned int strlen(const char *s);
void excepDivZero();
void excepInvalidOpcode();
int puts(const char* s);
int putChar(char);
char getChar();
int gets(char * s);
unsigned int charBelongs(char *s,char c);
int strcmp (const char *p1, const char *p2);
void getTime();
void printPrime();
void fibonacciNumbs();
void printmem();
void inforeg();
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

#endif