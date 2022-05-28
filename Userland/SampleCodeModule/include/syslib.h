#ifndef SYSLIB_H
#define SYSLIB_H

void sys_write(int fd, void *buffer, int size);
int sys_read(int fd,void* buffer,int size);
unsigned int strlen(const char *s);
void puts(const char* s);
void putChar(char);

#endif