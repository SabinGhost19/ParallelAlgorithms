#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include "Workers.h"

int startVertex = 0;
int destinationVertex = 3;

int N=10;
int printLevel;
int numVertices = 10;
int numEdges = 30;
int graphDefault[][2] = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
		{ 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
		{ 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
		{ 9, 7 } };
int **graph;

// Mutex pentru afisare thread-safe
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
volatile int solutionFound = 0;

// Structura pentru datele task-ului
typedef struct {
	int *partialPath;
	int step;
	int destination;
} PathTaskData;

void initDefaultGraph() {
	graph = (int**)malloc(sizeof(int*)*numEdges);
	for(int i = 0; i < numEdges; i++) {
		graph[i] = (int*)malloc(sizeof(int)*2);
		graph[i][0] = graphDefault[i][0];
		graph[i][1] = graphDefault[i][1];
	}
}

void generateGraph(int nVertices, int nEdges) {
	srand(42);
	numVertices = nVertices;
	numEdges = nEdges;
	graph = (int**)malloc(sizeof(int*)*numEdges);
	for(int i = 0; i < numEdges; i++) {
		graph[i] = (int*)malloc(sizeof(int)*2);
		graph[i][0] = rand()%numVertices;
		graph[i][1] = rand()%numVertices;
		if(graph[i][0] == graph[i][1])
			i--;
	}
}

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

void printAll(int * v, int N)
{
	int i;
	int max = 1;
	for(i = 0; i < N; i++)
		if(max < v[i])
			max = v[i];

	int displayWidth = 2 + log10(max);
	char * aux = malloc(1000*sizeof(char));
	aux[0] = '\0';
	char * vectorValue = malloc(10*sizeof(char));
	for(i = 0; i < N; i++) {
		sprintf(vectorValue, "%*i", displayWidth, v[i]);
		strcat(aux, vectorValue);
		strcat(aux, " ");
	}
	printf("%s\n", aux);
	free(aux);
	free(vectorValue);
}

void print(int * v, int N)
{
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printAll(v, N);
	else
		printAll(v, N);
}

int vectorContains(int * v, int size, int value) {
	for(int i=0;i<size;i++)
		if(v[i] == value)
			return 1;
	return 0;
}

// Functia task-ului pentru replicated workers
void getPathTask(void *data, int thread_id);

Task makePathTask(int *partialPath, int step, int destination) {
	Task task;
	PathTaskData *taskData = (PathTaskData*)malloc(sizeof(PathTaskData));

	// Copiem path-ul partial
	taskData->partialPath = (int*)malloc(sizeof(int) * 100);
	memcpy(taskData->partialPath, partialPath, sizeof(int) * 100);
	taskData->step = step;
	taskData->destination = destination;

	task.data = taskData;
	task.runTask = getPathTask;
	return task;
}

void getPathTask(void *data, int thread_id) {
	PathTaskData *taskData = (PathTaskData*)data;
	int *partialPath = taskData->partialPath;
	int step = taskData->step;
	int destination = taskData->destination;

	// Verificam daca am gasit deja o solutie
	if (solutionFound) {
		free(partialPath);
		free(taskData);
		return;
	}

	// Am ajuns la destinatie
	if (partialPath[step-1] == destination) {
		pthread_mutex_lock(&printMutex);
		if (!solutionFound) {
			solutionFound = 1;
			print(partialPath, step);
			forceShutDownWorkers();
		}
		pthread_mutex_unlock(&printMutex);
		free(partialPath);
		free(taskData);
		return;
	}

	// Incercam toate nodurile urmatoare posibile
	int lastNodeInPath = partialPath[step-1];
	for (int i = 0; i < numEdges; i++) {
		if (graph[i][0] == lastNodeInPath) {
			if (vectorContains(partialPath, step, graph[i][1]))
				continue;

			int * newPartialPath = malloc(sizeof(int) * 100);
			memcpy(newPartialPath, partialPath, sizeof(int) * 100);
			newPartialPath[step] = graph[i][1];

			// Cream un nou task in loc de apel recursiv
			putTask(makePathTask(newPartialPath, step + 1, destination));
			free(newPartialPath);
		}
	}

	free(partialPath);
	free(taskData);
}

int main(int argc, char** argv)
{
	getArgs(argc, argv);
	initDefaultGraph();
	//generateGraph(N, 2*N);

	int * partialPath = malloc(sizeof(int) * 100);
	memset(partialPath, 0, sizeof(int) * 100);
	partialPath[0] = startVertex;

	// Pornim workerii
	startWorkers();

	// Adaugam primul task
	putTask(makePathTask(partialPath, 1, destinationVertex));

	// Asteptam terminarea
	joinWorkerThreads();

	free(partialPath);
	return 0;
}
