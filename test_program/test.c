#include <pthread.h>
#include <stdio.h>

void *routine(void *param) { printf("Test routine\n"); }
int main() {
  pthread_t thread;

  pthread_create(&thread, NULL, &routine, NULL);

  pthread_join(thread, NULL);

  return 0;
}