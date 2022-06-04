#ifndef BASH_H
#define BASH_H

#include <syslib.h>

typedef void (*ptr)();
typedef ptr (*pm)();

void bash();
void help();
int readInput();
void unknownCommand();
void pipeManager();
ptr commandLine(char* buffer);

#endif
