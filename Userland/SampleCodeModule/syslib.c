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

char * gets(char * s){
    int i = 0, c = getChar();
    while (c != '\n' && c != EOF)
    {
        s[i++] = c;
        putChar(c);
        c = getChar();
    }
    s[i] = '\0';
    return (i != 0) ? s : NULL;
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
    puts("Prime numbers: ");
    while(1){
        if(isPrime(i) == 0){
            if(i<0){
            puts("\b\b");
            return;
            }   
            itoa(i,num);
            puts(num);
            puts(", ");
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
        if(nextTerm<0){
            puts("\b\b");
            return;
        }
        itoa(nextTerm,num);
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

