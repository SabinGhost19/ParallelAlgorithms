#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;

sem_t sem1, sem2;

void *threadFunction(void *var) {
  // TODO preserve the correct order by using semaphores you will need to add a
  // library and the correct flag when compiling
  int thread_id = *(int *)var;
  if (thread_id == 0) {
    sem_wait(&sem2);
    printf("I should be displayed last\n");
  } else if (thread_id == 1) {
    sem_wait(&sem1);
    printf("I should be displayed in the middle\n");
    sem_post(&sem2);

  } else if (thread_id == 2) {

    printf("I should be displayed first \n");
    sem_post(&sem1);
  }
}

void getArgs(int argc, char **argv) {}

void init() {
  sem_init(&sem1, 0, 0);
  sem_init(&sem2, 0, 0);
}

void printAll() {}

void printPartial() {}

void print() {
  if (printLevel == 0)
    return;
  else if (printLevel == 1)
    printPartial();
  else
    printAll();
}

int main(int argc, char *argv[]) {
  getArgs(argc, argv);
  init();

  P = 3; // ATTENTION, WE OVERWRITE THE NUMBER OF THREADS. WE ONLY NEED 3
  int i;

  pthread_t tid[P];
  int thread_id[P];
  for (i = 0; i < P; i++)
    thread_id[i] = i;
  // DO NOT EDIT
  for (i = 0; i < P; i++) {
    pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
  }
  // DO NOT EDIT
  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }
  // DO NOT EDIT
  print();

  return 0;
}
