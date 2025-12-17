#include "../headers/queue.h"

coada_taskuri_t coada;

// variabile interne pentru gestionarea terminarii
static pthread_mutex_t tasks_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tasks_cond = PTHREAD_COND_INITIALIZER;
static int tasks_remaining = 0;

void init_coada() {
    coada.size = 0;
    coada.capacity = 10000;
    coada.taskuri = (task_feed_t *)malloc(sizeof(task_feed_t) * coada.capacity);
    coada.head = 0;
    coada.tail = 0;
    coada.done = 0;
    pthread_mutex_init(&coada.mutex, NULL);
    pthread_cond_init(&coada.cond, NULL);
}

void adauga_task(int user_id) {
    pthread_mutex_lock(&coada.mutex);
    if (coada.size < coada.capacity) {
        coada.taskuri[coada.tail].user_id = user_id;
        coada.tail = (coada.tail + 1) % coada.capacity;
        coada.size++;
        pthread_cond_signal(&coada.cond);
    } else {
        fprintf(stderr, "eroare: coada plina\n");
    }
    pthread_mutex_unlock(&coada.mutex);
}

int extrage_task(task_feed_t *task) {
    pthread_mutex_lock(&coada.mutex);
    while (coada.size == 0 && !coada.done) {
        pthread_cond_wait(&coada.cond, &coada.mutex);
    }
    if (coada.size == 0 && coada.done) {
        pthread_mutex_unlock(&coada.mutex);
        return 0;
    }
    *task = coada.taskuri[coada.head];
    coada.head = (coada.head + 1) % coada.capacity;
    coada.size--;
    pthread_mutex_unlock(&coada.mutex);
    return 1;
}

void set_queue_done() {
    pthread_mutex_lock(&coada.mutex);
    coada.done = 1;
    pthread_cond_broadcast(&coada.cond);
    pthread_mutex_unlock(&coada.mutex);
}

void cleanup_coada() {
    free(coada.taskuri);
    pthread_mutex_destroy(&coada.mutex);
    pthread_cond_destroy(&coada.cond);
    pthread_mutex_destroy(&tasks_mutex);
    pthread_cond_destroy(&tasks_cond);
}

void increment_pending_tasks() {
    pthread_mutex_lock(&tasks_mutex);
    tasks_remaining++;
    pthread_mutex_unlock(&tasks_mutex);
}

void notifica_task_complet() {
    pthread_mutex_lock(&tasks_mutex);
    tasks_remaining--;
    if (tasks_remaining == 0) {
        pthread_cond_signal(&tasks_cond);
    }
    pthread_mutex_unlock(&tasks_mutex);
}

void wait_for_all_tasks() {
    pthread_mutex_lock(&tasks_mutex);
    while (tasks_remaining > 0) {
        pthread_cond_wait(&tasks_cond, &tasks_mutex);
    }
    pthread_mutex_unlock(&tasks_mutex);
}
