#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_func(void *arg) {
  while (1) {
  }
  return NULL;
}

int main() {
  pthread_t tid;
  int count = 0;

  while (pthread_create(&tid, NULL, thread_func, NULL) == 0) {
    count++;
    printf("thread creaeted %d\n", count);
  }

  printf("Total: %d\n", count);
  return 0;
}

// #include <math.h>
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>

// void *threadFunction(void *var) {
//   int thread_id = *(int *)var;

//   printf("Hello world from thread %i\n", thread_id);
// }

// int main(int argc, char **argv) {
//   int P = 100;
//   int i = 0;

//   pthread_t tid[P];
//   int thread_id[P];
//   for (i = 0; i < P; i++)
//     thread_id[i] = i;

//   //   for (i = 0; i < P; i++) {
//   //     int result =
//   //         pthread_create(&(tid[i]), NULL, threadFunction,
//   &(thread_id[i]));
//   //     if (result != 0) {
//   //       printf("Nr of maxim threads: %i\n", i);
//   //       break;
//   //     }
//   //   }

//   int count = 0;
//   while (pthread_create(&(tid[count]), NULL, threadFunction, NULL) == 0) {
//     count++;
//     printf("Am creat thread-ul %d\n", count);
//   }

//   printf("Nu mai pot crea thread-uri. Total: %d\n", count);

//   for (i = 0; i < P; i++) {
//     pthread_join(tid[i], NULL);
//   }

//   return 0;
// }
