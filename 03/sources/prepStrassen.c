#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;

int A11, A12, A21, A22;
int B11, B12, B21, B22;
int M[7];
int C11, C12, C21, C22;

typedef struct {
  int id;
} ThreadData;

void getArgs(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: ./program printLevel P\n");
    exit(1);
  }
  printLevel = atoi(argv[1]);
  P = atoi(argv[2]);
}

void initMatrices() {
  A11 = 3213215;
  A12 = 5454;
  A21 = 5456;
  A22 = 9898;
  B11 = 54544;
  B12 = 90821;
  B21 = 9807879;
  B22 = 329132;
}

void printResult() {
  if (printLevel == 0)
    return;
  printf("C = [%i %i; %i %i]\n", C11, C12, C21, C22);
}

void *computeMi(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  int id = data->id;

  switch (id) {
  case 0:
    M[0] = (A11 + A22) * (B11 + B22);
    break; // M1
  case 1:
    M[1] = (A21 + A22) * B11;
    break; // M2
  case 2:
    M[2] = A11 * (B12 - B22);
    break; // M3
  case 3:
    M[3] = A22 * (B21 - B11);
    break; // M4
  case 4:
    M[4] = (A11 + A12) * B22;
    break; // M5
  case 5:
    M[5] = (A21 - A11) * (B11 + B12);
    break; // M6
  case 6:
    M[6] = (A12 - A22) * (B21 + B22);
    break; // M7
  }

  free(data);
  return NULL;
}

int main(int argc, char **argv) {
  getArgs(argc, argv);
  initMatrices();

  if (P != 7) {
    printf("avem nevoie de 7 threads....constrangere, se aplica constrangerea: "
           "P=7\n");
    P = 7;
  }

  pthread_t threads[7];

  for (int i = 0; i < 7; i++) {
    ThreadData *data = malloc(sizeof(ThreadData));
    data->id = i;
    pthread_create(&threads[i], NULL, computeMi, data);
  }

  for (int i = 0; i < 7; i++) {
    pthread_join(threads[i], NULL);
  }

  C11 = M[0] + M[3] - M[4] + M[6];
  C12 = M[2] + M[4];
  C21 = M[1] + M[3];
  C22 = M[0] - M[1] + M[2] + M[5];

  printResult();
  return 0;
}
