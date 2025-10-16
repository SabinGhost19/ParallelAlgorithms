#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int NReps = 1;
int printLevel;
int N;
int P;
int* a;
int* b;
int* c;

typedef struct {
	int id;
	int start;
	int end;
} ThreadData;

void getArgs(int argc, char** argv)
{
	if (argc < 4) {
		printf("Not enough parameters: ./program N printLevel P\n");
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
	if (!a || !b || !c) {
		perror("malloc failed");
		exit(1);
	}

	for (int i = 0; i < N; i++) {
		a[i] = i % 500;
		b[i] = i % 500;
	}
}

void printPartial()
{
	for (int i = 0; i < 10; i++) printf("%i ", c[i]);
	printf("\n");
	for (int i = 20; i < N; i += N / 10) printf("%i ", c[i]);
	printf("\n");
	for (int i = N - 10; i < N; i++) printf("%i ", c[i]);
	printf("\n");
}

void printAll()
{
	for (int i = 0; i < N; i++) printf("%i ", c[i]);
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
	ThreadData* data = (ThreadData*)arg;
	for (int rep = 0; rep < NReps; rep++) {
		for (int i = data->start; i < data->end; i++) {
			c[i] = a[i] + b[i];
		}
	}
	free(data);
	return NULL;
}

int main(int argc, char* argv[])
{
	getArgs(argc, argv);
	init();

	pthread_t* threads = malloc(sizeof(pthread_t) * P);

	int chunk = N / P;
	for (int i = 0; i < P; i++) {
		ThreadData* data = malloc(sizeof(ThreadData));
		data->id = i;
		data->start = i * chunk;
		data->end = (i == P - 1) ? N : (i + 1) * chunk;
		pthread_create(&threads[i], NULL, thread_function, data);
	}

	for (int i = 0; i < P; i++) {
		pthread_join(threads[i], NULL);
	}

	print();

	free(a);
	free(b);
	free(c);
	free(threads);
	return 0;
}
