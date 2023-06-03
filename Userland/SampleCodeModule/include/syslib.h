#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdint.h>

#define NULL (void *)0

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define PIPEOUT 3
#define PIPEIN 4

#define EOF -1

#include <uniqueTypes.h>

int sys_write(int fd, const char *buffer, int size);
int sys_read(int fd, char *buffer, int size);
void sys_time(time_t *time);
int sys_getregs(uint64_t *registers);
void inv_opcode();
void div_zero();
void sys_copymem(uint64_t address, uint8_t *buffer, uint64_t length);
MemoryInfo *sys_memInfo();
extern void *sys_memMalloc(uint64_t size);
void sys_memFree(void *ap);
pid_t sys_exec(uint64_t program, unsigned int argc, char *argv[]);
pid_t sys_waitpid(pid_t pid);
int sys_kill(pid_t pid);
int sys_block(pid_t pid);
int sys_unblock(pid_t pid);
void sys_exit(int retValue);
sem_t sem_open(char *name, uint64_t value);
int sem_close(sem_t sem);
int sem_post(sem_t sem);
int sem_wait(sem_t sem);
int sys_yieldProcess();
int sys_nice(pid_t pid, int newPriority);
int sys_pipe(int pipefd[2]);
int sys_dup2(int fd1, int fd2);
int sys_open(int fd);
int sys_close(int fd);
processInfo * sys_ps();
int sys_changeProcessStatus(pid_t pid);
pid_t sys_getCurrentPid();
int sys_secondsElapsed();

unsigned int strlen(const char *s);
void excepDivZero();
void excepInvalidOpcode();
int puts(const char *s);
int putChar(char);
char getChar();
int gets(char *s);
unsigned int charBelongs(char *s, char c);
int containsString(const char *p1, const char *p2);
void savePrintMemParams(char *s);
int strcmp(const char *p1, const char *p2);
char **strtok(char *str, char delim, int *qty);
void strcpy(char *dest, char *src);
void fprintf(int fd, char *str, ...);
int is_num(char c);
int atoi(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

#endif