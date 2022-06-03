#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>

void _defaultExit();

void loadTasks(int (*program1)(), int (*program2)(), uint64_t * registers);

void nextTask(uint64_t * registers);

void pauseTask(unsigned int taskNum);

void exitTask(int retValue, uint64_t * registers);

#endif //SCHEDULER_H
