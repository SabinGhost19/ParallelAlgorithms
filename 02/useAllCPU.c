#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *threadFunction(void *var)
{
	int thread_id = *(int *)var;

	// TODO Write code to make me run for at least a minute
	printf("Thread %d START the pow computation\n", thread_id);
	double result = 0;
	for (int i = 0; i < 2000000000; i++) {
		result = pow(result, i);
	}
	printf("Thread %d FINISHED: %f\n", thread_id, result);
	return NULL;
}

int main(int argc, char **argv)
{
	int P = 16;
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

	return 0;
}
