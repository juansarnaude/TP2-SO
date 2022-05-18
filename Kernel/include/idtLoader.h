#ifndef _IDTLOADER_
#define _IDTLOADER_

#include <stdint.h>

void load_idt();

static void setup_IDT_entry (int index, uint64_t offset);

#endif