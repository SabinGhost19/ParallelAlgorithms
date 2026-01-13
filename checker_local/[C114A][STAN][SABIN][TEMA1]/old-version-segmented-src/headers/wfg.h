#ifndef WFG_H
#define WFG_H

#include "common.h"

typedef struct {
    int thread_idx;
    int waiting_for_user; // -1 daca nu asteapta
    int *held_users;      // lista de useri detinuti
    int nr_held;
    int cap_held;
} wfg_node_t;

typedef struct {
    wfg_node_t nodes[MAX_THREADS];
    pthread_mutex_t mutex;
    int nr_threads;
} wfg_t;

extern wfg_t wfg;

void wfg_init(int n_threads);
void wfg_set_waiting(int thread_idx, int user_id);
void wfg_add_held(int thread_idx, int user_id);
void wfg_remove_held(int thread_idx, int user_id);
int wfg_detect_deadlock(int thread_idx);
void wfg_cleanup();

#endif // WFG_H
