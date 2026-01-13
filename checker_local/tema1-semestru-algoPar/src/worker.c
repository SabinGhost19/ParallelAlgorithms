#include "../headers/worker.h"
#include "../headers/queue.h"
#include "../headers/feed.h"

void *worker_function(void *arg) {
    int thread_idx = *(int *)arg;
    free(arg);
    
    while (1) {
        task_feed_t task;
        if (!extrage_task(&task)) {
            break;
        }
        proceseaza_feed(thread_idx, task.user_id);
    }
    return NULL;
}
