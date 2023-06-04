
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

#include <naiveConsole.h>


static char* registers[] = {" RAX:", " RBX:", " RCX:", " RDX:", " RBP:", " RDI:", " RSI:", " R8:", " R9:", " R10:", " R11:", " R12:", " R13:", " R14:", " R15:"};

static void printRegisters(uint64_t rip, uint64_t rsp, const uint64_t * backup){
	ncNewline();
	ncPrintFormat("RIP:", ERROR_FORMAT);
	ncPrintHex(rip);
	ncPrintFormat(" RSP:", ERROR_FORMAT);
	ncPrintHex(rsp);
	for (int i = 14; i >= 0; i--)
	{
		ncPrintFormat(registers[14-i], ERROR_FORMAT);
		ncPrintHex(backup[i]);
	}
}