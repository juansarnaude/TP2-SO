#include <lib.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

//Wrapper function for printMem
int checkPrintMemParams(char *s,uint8_t** output){
    uint64_t size = strlen(s);//le resto el "printmem"
    if(size<3 || size>11 || s[0]!='0' || s[1]!='x'){
		ncPrint("\nIncorrect address format\n");
        return 0;
    }
	uint64_t dir=0;
    unsigned int i=2;
    while(s[i] != '\0' && i < 12){
        if((s[i] < '0' || s[i] > '9') && (s[i] < 'a' || s[i] > 'f')){
			ncPrint("\nAddress can't be accesed\n");
            return 0;
        }
        if(s[i]>='0' && s[i]<='9'){
			dir = 16*dir + s[i]-'0';
		}
		else if(s[i]>='a' && s[i]<='f'){
			if(i == 10){
				ncPrint("\nAddress can't be accesed\n");
				return 0;
			}else if(i == 11 && s[i-1] == '9' && s[i] > 'b'){
				ncPrint("\nAddress can't be accesed\n");
				return 0;
			}
			dir = 16*dir + s[i]-'a';
		}
        i++;
    }
	*output = (uint8_t*)dir;
    return 1;
}

void printmem(char* source)
{
	uint8_t* address;
    if(!checkPrintMemParams(source,&address)){
        return;
    }
    for(int i=0; i<32 ; i++){
        if(i%4==0){
            if(i%8==0){
				ncPrintChar('\n');
            }
        }
		ncPrintChar(valueToHexChar(address[i]>>4));
		ncPrintChar(valueToHexChar(address[i]&0x0F));
		ncPrintChar(' ');
		ncPrintChar(' ');
    }
    ncPrintChar(' ');
}

char valueToHexChar(unsigned char value) {
    return value >= 10 ? (value - 10 + 'A') : (value + '0');
}
