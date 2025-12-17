#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;

pthread_mutex_t mutex;
sem_t sem;
void *threadFunction(void *var) {
  int thread_id = *(int *)var;

  sem_wait(&sem);
  printf("There should be two messages displayed, I am one of them\n");
  sem_post(&sem);
  return NULL;
}
void getArgs(int argc, char **argv) {}

void init() { sem_init(&sem, 0, 1); }

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

  P = 2; // ATTENTION, WE OVERWRITE THE NUMBER OF THREADS. WE ONLY NEED 2
  int i;

  pthread_mutex_init(&mutex, NULL);
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
  pthread_mutex_destroy(&mutex);
  return 0;
}
