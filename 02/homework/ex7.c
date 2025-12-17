#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int** matrix_1 = NULL;
int** matrix_2 = NULL;
int** matrix_result = NULL;

int NReps;
int printLevel;
int NumberOfMatrixSquareSHape;
int N;
int P;

void getArgs(int argc, char** argv)
{
	if (argc < 4) {
		printf("Not enough paramters: ./program N printLevel P\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	NumberOfMatrixSquareSHape = atoi(argv[1]);
	N = NumberOfMatrixSquareSHape;
	printLevel = atoi(argv[2]);
	P = atoi(argv[3]);
}
void print_mat(int** mat)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}
void init()
{
	matrix_1 = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		matrix_1[i] = (int*)malloc(sizeof(int) * N);
	}

	matrix_2 = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		matrix_2[i] = (int*)malloc(sizeof(int) * N);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_1[i][j] = 1;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			matrix_2[i][j] = 2;
		}
	}

	matrix_result = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		matrix_result[i] = (int*)malloc(sizeof(int) * N);
	}
}
void* thread_function(void* arg)
{
	int thread_id = *(int*)arg;
	printf("THREAD: %d\n", thread_id);

	int start = thread_id * N / P;
	int end = (thread_id + 1) * N / P;

	for (int i = start; i < end; i++) {
		for (int j = 0; j < N; j++) {
			matrix_result[i][j] = matrix_1[i][j] + matrix_2[i][j];
		}
	}

	return NULL;
}
int main(int argc, char* argv[])
{
	getArgs(argc, argv);
	init();
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * P);
	int thread_id[P];
	for (int i = 0; i < P; i++)
		thread_id[i] = i;

	for (int k = 0; k < P; k++) {
		pthread_create(&threads[k], NULL, thread_function, &(thread_id[k]));
	}

	for (int k = 0; k < P; k++) {
		pthread_join(threads[k], NULL);
	}

	print_mat(matrix_result);

	return 0;
}