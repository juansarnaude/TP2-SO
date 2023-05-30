#include <timerDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	//nextTask(registers);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
