#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;
int a;
const int TOADD = 2;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *threadFunction(void *var) {
  int thread_id = *(int *)var;
  int i;
  printf("THreadId: %i\n", thread_id);

  // mutex added: begging and finish nof critical area
  pthread_mutex_lock(&lock);
  for (i = 0; i < N; i++)
    a = a + TOADD;
  pthread_mutex_unlock(&lock);

  printf("Thread %i finished: %i\n", thread_id, a);
  return NULL;
}

void getArgs(int argc, char **argv) {
  if (argc < 4) {
    printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, "
           "1=some, 2=verbouse\n");
    exit(1);
  }
  N = atoi(argv[1]);
  printLevel = atoi(argv[2]);
  P = atoi(argv[3]);
}

void init() { a = 0; }

void printAll() { printf("%i answer should be %i\n", a, P * N * TOADD); }

void printPartial() { printAll(); }

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

  pthread_t tid[P];
  int thread_id[P];
  for (i = 0; i < P; i++)
    thread_id[i] = i;

  for (i = 0; i < P; i++) {
    pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
  }

  for (i = 0; i < P; i++) {
    pthread_join(tid[i], NULL);
  }

  print();

  return 0;
}
