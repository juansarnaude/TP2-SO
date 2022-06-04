#include <irqDispatcher.h>


static void int_20(uint64_t * registers);
static void int_21(uint64_t * registers);

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	switch (irq) {
		case 0:
			int_20(registers);
			break;
		case 1:
			int_21(registers);
			break;
	}
	return;
}

static void int_20(uint64_t * registers) {
	timer_handler(registers);
}

static void int_21(uint64_t * registers) {
	keyboard_handler(registers);
}