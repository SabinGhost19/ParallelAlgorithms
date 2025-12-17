#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESULTS 10000

char *haystack;
char *needle;
int hayLen, needleLen;
int results[MAX_RESULTS];
int resCount = 0;
pthread_mutex_t resMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int start;
	int end;
} ThreadData;

void *searchSubstring(void *arg)
{
	ThreadData *data = (ThreadData *)arg;

	for (int i = data->start; i <= data->end - needleLen; i++) {
		if (strncmp(&haystack[i], needle, needleLen) == 0) {
			pthread_mutex_lock(&resMutex);
			if (resCount < MAX_RESULTS)
				results[resCount++] = i;
			pthread_mutex_unlock(&resMutex);
		}
	}

	free(data);
	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s haystack needle num_threads\n", argv[0]);
		return 1;
	}

	haystack = argv[1];
	needle = argv[2];
	int P = atoi(argv[3]);

	hayLen = strlen(haystack);
	needleLen = strlen(needle);

	if (P < 1) P = 1;
	pthread_t threads[P];

	int chunk = hayLen / P;

	for (int i = 0; i < P; i++) {
		ThreadData *data = malloc(sizeof(ThreadData));
		data->start = i * chunk;
		data->end = (i == P - 1) ? hayLen - 1 : (i + 1) * chunk + needleLen - 2;
		if (data->end >= hayLen) data->end = hayLen - 1;

		pthread_create(&threads[i], NULL, searchSubstring, data);
	}

	for (int i = 0; i < P; i++)
		pthread_join(threads[i], NULL);

	printf("Found at positions: ");
	for (int i = 0; i < resCount; i++)
		printf("%d ", results[i]);
	printf("\n");

	pthread_mutex_destroy(&resMutex);
	return 0;
}
