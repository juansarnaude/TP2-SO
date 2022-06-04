#ifndef INFOREG_H
#define INFOREG_h

#include <stdint.h>
#include <defs.h>

void setRegisters(uint64_t * registers);

uint64_t * getRegisters(); 

#endif
