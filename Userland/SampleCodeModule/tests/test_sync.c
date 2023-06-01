#include "test_util.h"
#include <syslib.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 32

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yieldProcess(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4)
    return;

  if ((n = atoi(argv[1])) <= 0)
    return;
  if ((inc = atoi(argv[2])) == 0)
    return;
  if ((use_sem = atoi(argv[3])) < 0)
    return;

  sem_t sem;

  if (use_sem)
    if ((sem = sem_open(SEM_ID, 1)) == 0) {
      fprintf(STDOUT, "test_sync: ERROR opening semaphore\n");
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sem_wait(sem);
    slowInc(&global, inc);
    if (use_sem)
      sem_post(sem);
  }

  if (use_sem)
    sem_close(sem);

  return;
}

void test_sync(int argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){
    fprintf(STDOUT, "ERROR: Invalid amount of arguments provided.\n");
    return;
  }

  char *argvDec[] = {"Decrement process", argv[1], "-1", argv[2]};
  char *argvInc[] = {"Increment process", argv[1], "1", argv[2]};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_exec((uint64_t) my_process_inc, 4, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_exec((uint64_t) my_process_inc, 4, argvInc);
  }

  //pid[0] = 2
  //pid[1] = 4
  //pid[2] = 3
  //pid[3] = 5
  fprintf(STDOUT, "test_sync: All processes created\n");
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_waitpid(pids[i]); //pid[0] = 2
    fprintf(STDOUT, "test_sync: PID %d finished\n", pids[i]);
    sys_waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
    fprintf(STDOUT, "test_sync: PID %d finished\n", pids[i + TOTAL_PAIR_PROCESSES]);
  }

  fprintf(STDOUT, "test_sync: Final value: %d\n", global);

  return;
}