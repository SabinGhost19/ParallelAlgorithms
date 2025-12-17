#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int printLevel;

void getArgs(int argc, char **argv)
{
	if(argc < 4) {
		printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	printLevel = atoi(argv[2]);
	P = atoi(argv[3]);
}

int resource = 0;

volatile int read_write_check = 0;
volatile int write_write_check = 0;
volatile int read_read_check = 0;
pthread_mutex_t rwmutex = PTHREAD_MUTEX_INITIALIZER;

int get() {
	read_read_check += 2;
	pthread_mutex_lock(&rwmutex);
	read_write_check += 2;
	pthread_mutex_unlock(&rwmutex);
	return resource;
}

void put(int value) {
	write_write_check += 2;
	read_write_check += 2;
	resource = value;
}


//HERE IS WHERE YOU NEED TO IMPLEMENT YOUR SOLUTION
pthread_mutex_t mutex_readcount = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_write = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;

int get_safe() {
	int aux;
	
	// Intrare cititor
	pthread_mutex_lock(&mutex_readcount);
	read_count++;
	if (read_count == 1) {
		// Primul cititor blocheaza scriitorii
		pthread_mutex_lock(&mutex_write);
	}
	pthread_mutex_unlock(&mutex_readcount);

	// Sectiune critica (citire)
	// Add a tiny delay to increase chance of overlap for the test check
	for(int k=0; k<100; k++); 
	aux = get();

	// Iesire cititor
	pthread_mutex_lock(&mutex_readcount);
	read_count--;
	if (read_count == 0) {
		// Ultimul cititor deblocheaza scriitorii
		pthread_mutex_unlock(&mutex_write);
	}
	pthread_mutex_unlock(&mutex_readcount);

	return aux;
}

void put_safe(int value) {
	// Intrare scriitor
	pthread_mutex_lock(&mutex_write);
	
	// Sectiune critica (scriere)
	put(value);

	// Iesire scriitor
	pthread_mutex_unlock(&mutex_write);
}
//END HERE IS WHERE YOU NEED TO IMPLEMENT YOUR SOLUTION


int value;
void* readerThread(void *var)
{
	int i;

	for (i = 0; i < N; i++) {
		value = get_safe();
	}

	return NULL;
}

void* writerThread(void *var)
{
	int i;

	for (i = 0; i < N; i++) {
		put_safe(i);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	getArgs(argc, argv);

	int i;
	int NREAD=P;
	int NWRITE=P;
	pthread_t tid[NREAD+NWRITE];

	for(i = 0; i < NREAD; i++) {
		pthread_create(&(tid[i]), NULL, readerThread, NULL);
	}

	for(; i < NREAD+NWRITE; i++) {
		pthread_create(&(tid[i]), NULL, writerThread, NULL);
	}

	for(i = 0; i < NREAD+NWRITE; i++) {
		pthread_join(tid[i], NULL);
	}

	if(N * 2 * P == read_read_check && P > 1) {
		printf("Failed two simultaneous readers\n");
		return 1;
	}
	if(N * 2 * P * 2 != read_write_check) {
		printf("Failed read when write %i %i \n", N * 2 * P * 2, read_write_check);
		return 1;
	}
	if(N * 2 * P != write_write_check) {
		printf("Failed write when write\n");
		return 1;
	}
	printf("Passed all\n");

	return 0;
}
