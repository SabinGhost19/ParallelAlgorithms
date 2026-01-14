#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <string.h>
#include "Workers.h"

int N;
int printLevel;
int COLORS = 3;
int numVertices = 10;
int numEdges = 30;
int graphDefault[][2] = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
			{ 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
			{ 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
			{ 9, 7 } };
int ** graph;

// Mutex pentru afisare thread-safe
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
volatile int solutionFound = 0;

// Structura pentru datele task-ului
typedef struct {
	int *colors;
	int step;
} ColorTaskData;

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

int isEdge(int a, int b) {
	for (int i = 0; i < numEdges; i++) {
		if (graph[i][0] == a && graph[i][1] == b)
			return 1;
	}
	return 0;
}

int verifyColors(int colors[], int step) {
	for (int i = 0; i < step; i++) {
		if (colors[i] == colors[step] && isEdge(i, step))
			return 0;
	}
	return 1;
}

// Functia task-ului pentru replicated workers
void colorGraphTask(void *data, int thread_id);

Task makeColorTask(int *colors, int step) {
	Task task;
	ColorTaskData *taskData = (ColorTaskData*)malloc(sizeof(ColorTaskData));

	taskData->colors = (int*)malloc(sizeof(int) * numVertices);
	memcpy(taskData->colors, colors, sizeof(int) * numVertices);
	taskData->step = step;

	task.data = taskData;
	task.runTask = colorGraphTask;
	return task;
}

void colorGraphTask(void *data, int thread_id) {
	ColorTaskData *taskData = (ColorTaskData*)data;
	int *colors = taskData->colors;
	int step = taskData->step;

	// Verificam daca am gasit deja o solutie
	if (solutionFound) {
		free(colors);
		free(taskData);
		return;
	}

	// Am colorat toate nodurile
	if (step == numVertices) {
		pthread_mutex_lock(&printMutex);
		if (!solutionFound) {
			solutionFound = 1;
			print(colors, step);
			forceShutDownWorkers();
		}
		pthread_mutex_unlock(&printMutex);
		free(colors);
		free(taskData);
		return;
	}

	// Incercam toate culorile pentru nodul curent
	for (int i = 0; i < COLORS; i++) {
		int * newColors = (int*)malloc(sizeof(int) * numVertices);
		memcpy(newColors, colors, sizeof(int) * numVertices);

		newColors[step] = i;
		if (verifyColors(newColors, step)) {
			// Cream un nou task in loc de apel recursiv
			putTask(makeColorTask(newColors, step + 1));
		}
		free(newColors);
	}

	free(colors);
	free(taskData);
}

int main(int argc, char** argv)
{
	getArgs(argc, argv);
	initDefaultGraph();
	//generateGraph(N, 2*N);

	int * colors = (int*)malloc(sizeof(int) * numVertices);
	memset(colors, 0, sizeof(int) * numVertices);

	// Pornim workerii
	startWorkers();

	// Adaugam primul task
	putTask(makeColorTask(colors, 0));

	// Asteptam terminarea
	joinWorkerThreads();

	free(colors);
	return 0;
}
