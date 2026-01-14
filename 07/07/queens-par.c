#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Workers.h"

int N;
int printLevel;

// Mutex pentru afisare thread-safe
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
volatile int solutionFound = 0;

typedef struct partialSolution {
	int* queens;
	int position;
	int step;
} partialSolution;

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

int canReject(partialSolution PS) {
	for (int i = 0; i < PS.step; i++) {
		if (PS.queens[i] == PS.queens[PS.step])
			return 1;
		if (PS.queens[i] - PS.queens[PS.step] == i - PS.step)
			return 1;
		if (PS.queens[PS.step] - i == PS.queens[i] - PS.step)
			return 1;
	}
	return 0;
}

int isSolution(partialSolution PS) {
	if (PS.step < N - 1)
		return 0;
	return !canReject(PS);
}

void printAll(partialSolution PS) {
	for (int i = 0; i < N; i++)
		printf("%3i ", PS.queens[i]);
	printf("\n");
}

void print(partialSolution PS)
{
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printAll(PS);
	else
		printAll(PS);
}

partialSolution increaseStep(partialSolution PS) {
	partialSolution newPS;
	newPS.queens = malloc(sizeof(int) * N);
	memcpy(newPS.queens, PS.queens, sizeof(int) * N);
	newPS.step = PS.step+1;
	newPS.position = 0;
	return newPS;
}

partialSolution getNextPartialSolution(partialSolution PS) {
	partialSolution newPS;
	newPS.queens = malloc(sizeof(int) * N);
	memcpy(newPS.queens, PS.queens, sizeof(int) * N);
	newPS.queens[PS.step]++;
	newPS.position = PS.position+1;
	newPS.step = PS.step;
	return newPS;
}

int hasSolutionAtStep(partialSolution PS) {
	return (PS.step < N) && (PS.position < N);
}

// Functia task-ului pentru replicated workers
void backtrackingTask(void *data, int thread_id);

Task makeQueensTask(partialSolution PS) {
	Task task;
	partialSolution *taskData = (partialSolution*)malloc(sizeof(partialSolution));

	taskData->queens = malloc(sizeof(int) * N);
	memcpy(taskData->queens, PS.queens, sizeof(int) * N);
	taskData->position = PS.position;
	taskData->step = PS.step;

	task.data = taskData;
	task.runTask = backtrackingTask;
	return task;
}

void backtrackingTask(void *data, int thread_id) {
	partialSolution *taskData = (partialSolution*)data;
	partialSolution PS = *taskData;

	// Verificam daca am gasit deja o solutie
	if (solutionFound) {
		free(taskData->queens);
		free(taskData);
		return;
	}

	if (canReject(PS)) {
		free(taskData->queens);
		free(taskData);
		return;
	}

	if (isSolution(PS)) {
		pthread_mutex_lock(&printMutex);
		if (!solutionFound) {
			solutionFound = 1;
			print(PS);
			forceShutDownWorkers();
		}
		pthread_mutex_unlock(&printMutex);
		free(taskData->queens);
		free(taskData);
		return;
	}

	PS = increaseStep(PS);
	while (hasSolutionAtStep(PS)) {
		PS = getNextPartialSolution(PS);
		// Cream un nou task in loc de apel recursiv
		putTask(makeQueensTask(PS));
	}

	free(taskData->queens);
	free(taskData);
}

int main(int argc, char** argv)
{
	getArgs(argc, argv);

	partialSolution initialValues;
	initialValues.queens = malloc(sizeof(int) * N);
	for(int i = 0; i < N; i++)
		initialValues.queens[i] = -1;
	initialValues.position = -1;
	initialValues.step = -1;

	// Pornim workerii
	startWorkers();

	// Adaugam primul task
	putTask(makeQueensTask(initialValues));

	// Asteptam terminarea
	joinWorkerThreads();

	free(initialValues.queens);
	return 0;
}
