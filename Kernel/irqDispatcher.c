#include <irqDispatcher.h>


static void int_20(uint64_t * registers);
static void int_21();

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	switch (irq) {
		case 0:
			int_20(registers);
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

static void int_20(uint64_t * registers) {
	timer_handler(registers);
}

void int_21() {
	keyboard_handler();
}