#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include <keyboardDriver.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBINT 3

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax);
#endif