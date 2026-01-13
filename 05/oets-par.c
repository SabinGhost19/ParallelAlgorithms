#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int printLevel;
int N;
int P;
int *v;
int *vQSort;
int *vNew;
int tmp_par = 0;
int tmp_impar = 1;

pthread_barrier_t barier;

void compareVectors(int *a, int *b)
{
	// DO NOT MODIFY
	int i;
	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("%d mine vs %d\n ", a[i], b[i]);
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int *v)
{
	// DO NOT MODIFY
	int i;
	int max = 1;
	for (i = 0; i < N; i++)
		if (max < log10(v[i]))
			max = log10(v[i]);
	int displayWidth = 2 + max;
	for (i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
		if (!((i + 1) % 20))
			printf("\n");
	}
	printf("\n");
}

int cmp(const void *a, const void *b)
{
	// DO NOT MODIFY
	int A = *(int *)a;
	int B = *(int *)b;
	return A - B;
}

void getArgs(int argc, char **argv)
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
	pthread_barrier_init(&barier, NULL, P);
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	if (v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for (i = 0; i < N; i++)
		v[i] = rand() % N;
}

void printPartial()
{
	int i;
	compareVectors(v, vQSort);
}

void printAll()
{
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v, vQSort);
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
void *threadFunction(void *arg)
{
	int id = *(int *)arg;

	// Calculam start si end pentru acest thread
	// Numarul total de perechi este (N-1)/2 pentru fiecare faza
	int total_pairs = (N + 1) / 2;
	int pairs_per_thread = total_pairs / P;
	int remainder = total_pairs % P;

	int my_start = id * pairs_per_thread + (id < remainder ? id : remainder);
	int my_end = my_start + pairs_per_thread + (id < remainder ? 1 : 0);

	for (int phase = 0; phase < N; phase++) {
		if (phase % 2 == 0) {
			// Faza para: comparam perechile (0,1), (2,3), (4,5), ...
			for (int p = my_start; p < my_end; p++) {
				int j = p * 2;  // pozitia elementului din stanga
				if (j < N - 1 && v[j] > v[j + 1]) {
					int tmp = v[j];
					v[j] = v[j + 1];
					v[j + 1] = tmp;
				}
			}
		} else {
			// Faza impara: comparam perechile (1,2), (3,4), (5,6), ...
			for (int p = my_start; p < my_end; p++) {
				int j = p * 2 + 1;  // pozitia elementului din stanga (incepand de la 1)
				if (j < N - 1 && v[j] > v[j + 1]) {
					int tmp = v[j];
					v[j] = v[j + 1];
					v[j + 1] = tmp;
				}
			}
		}
		pthread_barrier_wait(&barier);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int i, j, aux;
	getArgs(argc, argv);
	init();

	// make copy to check it against qsort
	// DO NOT MODIFY
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// sort the vector v
	// PARALLELIZE ME
	// int sorted = 0;
	// while (!sorted) {
	// 	sorted = 1;
	// 	for (i = 0; i < N - 1; i++) {
	// 		if (v[i] > v[i + 1]) {
	// 			aux = v[i];
	// 			v[i] = v[i + 1];
	// 			v[i + 1] = aux;
	// 			sorted = 0;
	// 		}
	// 	}
	// }
	// displayVector(v);

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
