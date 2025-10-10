#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int NReps;
int printLevel;
int N;
int P;
int* a;
int* b;
int* c;

void getArgs(int argc, char** argv)
{
	if (argc < 4) {
		printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	printLevel = atoi(argv[2]);
	P = atoi(argv[3]);
}

void init()
{
	a = malloc(sizeof(int) * N);
	b = malloc(sizeof(int) * N);
	c = malloc(sizeof(int) * N);
	if (a == NULL || b == NULL || c == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	int i;
	for (i = 0; i < N; i++) {
		a[i] = i % 500;
		b[i] = i % 500;
	}
}

void printPartial()
{
	int i;
	for (i = 0; i < 10; i++)
		printf("%i ", c[i]);
	printf("\n");
	for (i = 20; i < N; i += N / 10)
		printf("%i ", c[i]);
	printf("\n");
	for (i = N - 10; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void printAll()
{
	int i;
	for (i = 0; i < N; i++)
		printf("%i ", c[i]);
	printf("\n");
}

void print()
{
	if (printLevel == 0)
		return;
	else if (printLevel == 1)
		printPartial();
	else
		printAll();
}
void* thread_function(void* arg)
{
	int thread_id = *(int*)arg;
	printf("THREAD: %d\n", thread_id);
	printf("Thread %d start comp from: %d to %d\n", thread_id, (N / P) * thread_id, (N / P) * (thread_id + 1));
	for (int j = (N / P) * thread_id; j < (N / P) * (thread_id + 1); j++) {
		c[j] = a[j] + b[j];
	}
	return NULL;
}
int main(int argc, char* argv[])
{
	int i, j;
	getArgs(argc, argv);
	init();

	// // TODO paralelize me
	// for (j = 0; j < NReps; j++)
	// 	for (i = 0; i < N; i++)
	// 		c[i] = a[i] + b[i];

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * P);
	int thread_id[P];
	for (i = 0; i < P; i++)
		thread_id[i] = i;

	for (int k = 0; k < P; k++) {
		pthread_create(&threads[k], NULL, thread_function, &(thread_id[k]));
	}

	for (int k = 0; k < P; k++) {
		pthread_join(threads[k], NULL);
	}

	print();

	return 0;
}
