#include <syslib.h>

unsigned int strlen(const char *str){
    unsigned int len = 0;
    while(str[len]!='\0')
    {
        len++;
    }
    return len;
}

int puts(const char * str){
    int len = strlen(str);
    return sys_write(STDOUT, str, len);
}

int putChar(char c){
    return sys_write(STDOUT, &c, 1);
}

int gets(char * s){
    int i = 0, c = getChar();
    while (c != '\n' && c != EOF)
    {
        s[i] = c;
        if(s[i] == '\b' && i > 0){
            i--;
            putChar(c);
        }else if(s[i] == '\b' && i == 0 ){
            //no hago nada
        }else{
            i++;
            putChar(c);
        }
        c = getChar();
    }
    s[i] = '\0';
    return i;
}

void getTime(){
    sys_time();
}

char getChar(){
    char c;
    while (sys_read(STDIN, &c, 1) == 0)
    {
        ;
    }
    
    return c;
}

//Retorna 1 si lo encuentra, 0 sino
unsigned int charBelongs(char *s,char c){
    while(*s != '\0'){
        if(*s == c){
            return 1;
        }
        s++;
    }
    return 0;
}

void excepDivZero(){
    int i=7/0;
}

void excepInvalidOpcode(){
    inv_opcode();
}

//https://code.woboq.org/userspace/glibc/string/strcmp.c.html
int strcmp (const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

static void reverse(char s[]);

//Turn an integer number to a char array
void itoa(int n, char s[]){
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}

static void reverse(char s[]){
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

//Devuelve 1 si es primo, 0 si no lo es.
int isPrime(int n)
{
	int i;
	for(i=2;i<=n/2;i++)
	{
		if(n%i==0)
			return 0;
	}
	return 1;
}

//Ciclo infinito que imprime numeros primos
void printPrime(){
    char num[30];
    int i=2;
    puts("Prime numbers: ");
    puts("1, ");
    while(1){
        if(isPrime(i)){
            if(i<0){
            puts("\b\b");
            return;
            }   
            itoa(i,num);
            puts(num);
            puts(",\n");
        }
        i++;
    }
}

//Ciclo infinito que imprime numeros de secuencia de fibonacci
void fibonacciNumbs(){
    char num[30];
    int i=3;
    int t1 = 0, t2 = 1;
    int nextTerm = t1 + t2;
    puts("Fibonacci Series: 0, 1, ");
    while(1) {
        if(nextTerm<0){//por si se pasa del max integer
            puts("\b\b");
            return;
        }
        itoa(nextTerm,num);
        puts(num);
        puts(",\n");
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
        i++;
    }
}

static char valueToHexChar(unsigned char value);

//Checks the first occurence of p2 inside p1, return the index where p2 appears inside p1 or -1
//if p2 isnt found inside p1
int containsString(const char *p1,const char *p2){
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    if(s2 == NULL)//if s2 is empty
        return -1;
    unsigned int i=0;
    unsigned char flag=0;
    while(*s1 != '\0' && !flag){
        if(*s1 == *s2){
            flag =1;
        }
        i++;s1++;
    }
    if(!flag){
        return -1;
    }
    while(*s1 != '\0' && *s2 != '\0'){
        if(*s1 != *s2){
            return -1;
        }
        s1++;s2++;
    }
    if(*s2 == '\0'){
        return i;
    }
    return -1;
}

static char address[18];

//Wrapper function for printMem
int checkPrintMemParams(char *s){
    s += 8;//me paro despues del "printmem"
    unsigned int i=0;
    while(*s != '\0' && i < 18){
        address[i++] = *s;
        s++;
    }
    if(i == 18){
        unknownCommand();
        return -1;
    }
    int j=0;
    while(j < i){
        if((address[j] < '0' || address[j] > '9') && (address[j] < 'a' || address[j] > 'f')){
            puts("Address can't be accesed");
            return -1;
        }
        j++;
    }
    return 1;
}


void printmem(unsigned char *source)
{
    for(int i=0; i<32 ; i++){
        if(i%4==0){
            if(i%8==0){
                putChar('\n');
            }else{
                putChar('\t');
            }
        }
        putChar(valueToHexChar(source[i]>>4));
        putChar(valueToHexChar(source[i]&0x0F));
        putChar(' ');
        putChar(' ');
    }
    putChar('\n');
}

static char valueToHexChar(unsigned char value) {
    return value >= 10 ? (value - 10 + 'A') : (value + '0');
}

void inforeg(){
    static char* registers[18] = { "RSP", "RFL", "RIP", "R15", "R14", "R13", "R12", "R11", "R10", "R9 ", "R8 ", "RSI", "RDI", "RBP", "RDX", "RCX", "RBX", "RAX"};
    uint64_t regval[18];
    int sysret = sys_getregs(regval);
    if (sysret == 0){
        puts("No registers to print.\n");
        return;
    } 
    char buffer[64] = {'0'};
    for(int i=0;i<18;i++){
        puts(registers[i]);
        puts(": 0x");
        uintToBase(regval[i], buffer, 16);
        puts(buffer);
        putChar('\n');
    }
}

//Tomado de x86-Barebones
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
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
