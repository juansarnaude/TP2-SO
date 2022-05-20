#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>
#include <lib.h>
#include <dates.h>

void ncPrintTime();
void ncDeleteChar();
void ncPrintFormat(const char* string,uint8_t format);
void ncPrintCharFormat(char character,uint8_t format);
void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

#endif