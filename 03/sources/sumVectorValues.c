#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;
long long *v;
long long sum;
pthread_mutex_t sumMutex = PTHREAD_MUTEX_INITIALIZER;

void getArgs(int argc, char **argv) {
  if (argc < 4) {
    printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, "
           "1=some, 2=verbouse\n");
    exit(1);
  }
  N = atoi(argv[1]);
  printLevel = atoi(argv[2]);
  P = atoi(argv[3]);

  printf("nr NUMBERE: %d...nr print l;ebe:%d.....P-threds: %d...\n", N,
         printLevel, P);
}

long long *allocVector(int N) {
  long long *v = malloc(sizeof(long long) * N);
  if (v == NULL) {
    printf("malloc failed!");
    exit(1);
  }
  return v;
}

void init() {
  v = allocVector(N);

  long long i;
  for (i = 0; i < N; i++) {
    v[i] = i + 1;
  }
}

void printPartial() { printf("Sum: %lli \n", sum); }

void printAll() { printPartial(); }

void print() {
  if (printLevel == 0)
    return;
  else if (printLevel == 1)
    printPartial();
  else
    printAll();
}
void *computeVector(void *arg) {

  int threadId = *(int *)arg;
  free(arg);

  int start = threadId * (N / P);
  int end = (N / P) * (threadId + 1);

  printf("Thread %i is running: start=%d and end=%d\n", threadId, start, end);
  pthread_mutex_lock(&sumMutex);
  for (int i = start; i < end; i++) {
    sum += v[i];
  }
  pthread_mutex_unlock(&sumMutex);

  return NULL;
}

int main(int argc, char *argv[]) {
  getArgs(argc, argv);
  init();
  pthread_t *threadVec = (pthread_t *)malloc(sizeof(pthread_t) * P);

  for (int i = 0; i < P; i++) {
    int *id = malloc(sizeof(int));
    *id = i;
    if (pthread_create(&threadVec[i], NULL, computeVector, id) != 0) {
      perror("pthread_create failed");
      exit(1);
    }
  }

  for (int i = 0; i < P; i++) {
    pthread_join((threadVec[i]), NULL);
  }
  print();

  return 0;
}
