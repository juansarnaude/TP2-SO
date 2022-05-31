#include <naiveConsole.h>

#define WIDTH 80
#define HEIGHT 25
#define VIDEOSTART 0xB8000

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *)VIDEOSTART;
static uint8_t *currentVideo = (uint8_t *)VIDEOSTART;
static const uint32_t width = WIDTH;
static const uint32_t height = HEIGHT;

//Window currently writing to.
static uint8_t currentWindow = 0;
//Amount of windows
static uint8_t windows = 1;
//Starting point for windows
static uint8_t *videoWindow[2] = {(uint8_t*)VIDEOSTART, (uint8_t*)VIDEOSTART + WIDTH};
//Pointer to windows
static uint8_t *currentVideoW[2] = {(uint8_t*)VIDEOSTART, (uint8_t*)VIDEOSTART + WIDTH};
//Default format color
static const uint8_t color = 0x07;

//Select amount of windows
uint8_t ncWindows(uint8_t amount){
	if (amount >= 2){
		if (windows != 2)
			ncClear();
		return windows = 2;
	}
	if (windows != 1)
		ncClear();
	return windows = 1;
}

//Select windows to write to
uint8_t ncCurrentWindow(uint8_t select){
	if (select > 0)
		return currentWindow = 1;
	return currentWindow = 0;
}

void ncPrintTime()
{
	ncPrint("  Date:");
	ncPrintDec(localDay());
	ncPrint("/");
	ncPrintDec(localMonth());
	ncPrint("/20");
	ncPrintDec(localYear());
	ncNewline();
	ncPrint("  Hours:");
	ncPrintDec(localHours()); // restamos 3 ya que la hora argentina es UTC-03:00
	ncNewline();
	ncPrint("  Minutes:");
	ncPrintDec(getMinutes());
	ncNewline();
	ncPrint("  Seconds:");
	ncPrintDec(getSeconds());
	ncNewline();
}
void ncDeleteChar(){
	if (windows == 1){
		if (currentVideo-2 > video){
			currentVideo -= 2;
			*currentVideo = ' ';
		}
		return;
	} else {
		if (currentVideoW[currentWindow]-2 > videoWindow[currentWindow]){
			currentVideoW[currentWindow] -= 2;
			*(currentVideoW[currentWindow]) = ' ';
		}
	}
}

void ncPrintFormat(const char* string,uint8_t format){
	for(int i=0;string[i]!='\0';i++){
		ncPrintCharFormat(string[i],format);
	}
}

void ncPrintCharFormat(char character,uint8_t format){
	switch (character)
	{
	case '\n':
		ncNewline();
		return;
		break;
	case '8':
		ncDeleteChar();
		return;
		break;
	default:
		break;
	}
	if (windows == 1){
		*(currentVideo++) = character;
		*(currentVideo++) = format;
	} else {
		*(currentVideoW[currentWindow]++) = character;
		*(currentVideoW[currentWindow]++) = format;
		if (currentWindow && ((currentVideoW[currentWindow] - video) % (2*width) <= width)){
			currentVideoW[currentWindow] += width;
		} else if (!currentWindow && ((currentVideoW[currentWindow] - video) % (2*width) >= width))
			currentVideoW[currentWindow] += width;
	}
}

void ncPrint(const char *string)
{
	int i; 

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	ncPrintCharFormat(character, color);
}

void ncNewline()
{
	if (windows == 1){
		do
		{
			ncPrintChar(' ');
		} while ((uint64_t)(currentVideo - video) % (width * 2) != 0);
	} else {
		do
		{
			ncPrintChar(' ');
		} while ((uint64_t)(currentVideoW[currentWindow] - video) % (width * 2) != ( currentWindow ? width : 0 ));
		
	}
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncClear()
{
	
	if (windows==1){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
	}

	else{
		ncClearWindow(currentWindow);
	}
}

void ncClearWindow(uint8_t windowToCLear){
	int i;
	uint32_t widthW;
	if (windowToCLear==0){
		widthW=width/2;
		i=0;
	}
	else{
		widthW=width;
		i=width/2 +1 ;
	}
		
	while(i < height*widthW){
		video[i*2]=' ';
		if (i && (i+1)%widthW==0)
			i+=width/2;
		i++;
	}
	currentVideoW[windowToCLear]=video;
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
