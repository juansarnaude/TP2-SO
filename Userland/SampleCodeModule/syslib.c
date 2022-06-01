#include <syslib.h>

unsigned int strlen(const char *s){
    unsigned int count = 0;
    while(*s!='\0')
    {
        count++;
        s++;
    }
    return count;
}

unsigned int scanf(char *s,unsigned int size){
    int i=0;
    while(i < size){
        char c =getChar();
        if('\n' == c){
            s[i] = '\0';
            return i;
        }
        putChar(c);
        s[i] = c;
        i++;
    }
    return i;
}

void getTime(){
    sys_time();
}

char getChar(){
    char c;
    sys_read(0,&c,1);
    return c;
}

void putChar(char c){
    sys_write(1,&c,1);
}

void puts(const char* s){
    while(*s != '\0'){
        putChar(*s);
        s++;
    }
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

void reverse(char s[]){
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

//Ciclo infinito que imprime numeros primos
void printPrime(){
    char num[30];
    int i=2;
    while(1){
        if(isPrime(i) == 0){
            itoa(i,num);
            puts(num);
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
        itoa(num,nextTerm);
        puts(num);
        puts(", ");
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
        i++;
    }
}

//Devuelve 0 si es primo, 1 si no lo es.
int isPrime(int n)
{
	int i;
	for(i=2;i<=n/2;i++)
	{
		if(n%i==0)
			return 1;
	}
	return 0;
}

//solucion momentanea para interpretar comandos
int strcmpBrazil (const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0' || c2 == '\0')
        return 0;;
    }
  while (c1 == c2);
  return c1 - c2;
}


    
void help(){
    const char* helpstring = 
    "HELP                 Provides help information for commands.\n"
	"CLEAR                Clears the console.\n"
	"DIVIDEBYZERO         Command to verify the operation of the exception routine \"Divide by zero\"\n"
    "INVALIDOPCODE        Command to verify the operation of the exception routine \"Invalid Opcode\"\n"
	"INFOREG              Prints on screen the value of all registers.\n"
	"PRINTMEM             Receives as argument a pointer and performs a memory dump of 32 bytes from the\n"
	"                     address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n";
    "PRIMENUMBS           Dispalys prime numbers starting from 1.\n"
    "FIBONACCINUMBS       Dispalys fibonacci series numbers.\n"
    "COMAND1 | COMAND2    The \"|\" operand allows the execution of multimple comands at the same time.\n";
    puts(helpstring);
}

