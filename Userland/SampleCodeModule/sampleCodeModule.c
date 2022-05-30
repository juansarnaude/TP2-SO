/* sampleCodeModule.c */

#include <syslib.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	bash();
	return 0x600DB7E;
}