#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>

int executeTask(int (*program)());

void nextTask(uint64_t * registers);

void pauseTask(unsigned int taskNum);

void exitTask();

#endif //SCHEDULER_H
