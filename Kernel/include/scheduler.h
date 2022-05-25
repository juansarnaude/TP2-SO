#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>

int executeTask(int (*program)(int argc, char const * argv[]), int argc, char const *argv[]);

void nextTask(uint64_t * registers);

void exitTask();

#endif //SCHEDULER_H
