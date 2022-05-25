#ifndef timerDriver
#define timerDriver

#include <idtLoader.h>
#include <naiveConsole.h>
#include <scheduler.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void printEvery5s();

#endif
