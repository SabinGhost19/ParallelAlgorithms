#define _XOPEN_SOURCE 700
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int N;
int P;

pthread_mutex_t mutex;

pthread_mutexattr_t attr;
int printLevel = 0;

void *threadFunction(void *var) {
  // TODO preserve the correct order by using barriers
  int thread_id = *(int *)var;
  if (thread_id == 0) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    printf("There should be two messages displayed, I am one of them\n");

  } else {
    printf("There should be two messages displayed, I am one of them\n");
  }
}

void getArgs(int argc, char **argv) {}

void init() {}

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

  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex, &attr);

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
  pthread_mutexattr_destroy(&attr);

  return 0;
}
