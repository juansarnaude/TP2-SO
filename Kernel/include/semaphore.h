#include <unistd.h>
#include <stdint.h>

typedef struct {
    char * name;
    uint64_t value;
    char locked;            //if its locked its 1 if not 0;
} semaphore;
 
typedef semaphore * sem_t;

sem_t sem_open(char * name, uint64_t value);
int sem_close(sem_t sem);
int sem_post(sem_t sem);
int sem_wait(sem_t sem);