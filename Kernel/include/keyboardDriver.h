#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <idtLoader.h>
#include <naiveConsole.h>
#include <lib.h>

void key_handler();

uint64_t readBuffer(char* output,unsigned int count);

#endif 