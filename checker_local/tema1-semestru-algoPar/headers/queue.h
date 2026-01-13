#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

typedef struct {
    int user_id;
} task_feed_t;

typedef struct {
    task_feed_t *taskuri;
    int size;
    int capacity;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int done; 
} coada_taskuri_t;

extern coada_taskuri_t coada;

void init_coada();
void adauga_task(int user_id);
int extrage_task(task_feed_t *task);
void set_queue_done();
void cleanup_coada();

// functii pentru gestionarea terminarii task-urilor
void increment_pending_tasks();
void notifica_task_complet();
void wait_for_all_tasks();

#endif // QUEUE_H
