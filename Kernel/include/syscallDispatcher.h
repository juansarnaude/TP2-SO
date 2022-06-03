#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include <keyboardDriver.h>
#include <scheduler.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t * registers);

#endif