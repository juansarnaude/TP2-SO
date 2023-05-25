#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <timerDriver.h>
#include <defs.h>
#include <queue.h>

void dummyProcess();
void createScheduler();
PCB *getProcess(pid_t pid);
uint64_t getCurrentPid();
int blockProcess(pid_t pid);
int unblockProcess(pid_t pid);
pid_t createProcess(uint64_t rip, int argc, char *argv[]);
void nextProcess();
void prepareDummyForWork();
uint64_t contextSwitch(uint64_t rsp);
int killProcess(int returnValue);
int changePriority(pid_t pid, int priorityValue);

#endif // SCHEDULER_H