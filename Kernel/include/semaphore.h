#include <unistd.h>
#include <stdint.h>
#include <defs.h>

sem_t sem_open(char * name, uint64_t value);
int sem_close(sem_t sem);
int sem_post(sem_t sem);
int sem_wait(sem_t sem);