#include <timerDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	//La interrupción tiene que terminar en el nuevo programa
	//Fuck
	nextTask();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
