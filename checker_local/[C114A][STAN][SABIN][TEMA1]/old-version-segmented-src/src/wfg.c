#include "../headers/wfg.h"

wfg_t wfg;

void wfg_init(int n_threads) {
    wfg.nr_threads = n_threads;
    pthread_mutex_init(&wfg.mutex, NULL);
    for (int i = 0; i < n_threads; i++) {
        wfg.nodes[i].thread_idx = i;
        wfg.nodes[i].waiting_for_user = -1;
        wfg.nodes[i].cap_held = 10;
        wfg.nodes[i].nr_held = 0;
        wfg.nodes[i].held_users = (int *)malloc(sizeof(int) * wfg.nodes[i].cap_held);
    }
}

void wfg_set_waiting(int thread_idx, int user_id) {
    pthread_mutex_lock(&wfg.mutex);
    wfg.nodes[thread_idx].waiting_for_user = user_id;
    pthread_mutex_unlock(&wfg.mutex);
}

void wfg_add_held(int thread_idx, int user_id) {
    pthread_mutex_lock(&wfg.mutex);
    wfg_node_t *n = &wfg.nodes[thread_idx];
    if (n->nr_held == n->cap_held) {
        n->cap_held *= 2;
        n->held_users = (int *)realloc(n->held_users, sizeof(int) * n->cap_held);
    }
    n->held_users[n->nr_held++] = user_id;
    pthread_mutex_unlock(&wfg.mutex);
}

void wfg_remove_held(int thread_idx, int user_id) {
    pthread_mutex_lock(&wfg.mutex);
    wfg_node_t *n = &wfg.nodes[thread_idx];
    for (int i = 0; i < n->nr_held; i++) {
        if (n->held_users[i] == user_id) {
            n->held_users[i] = n->held_users[n->nr_held - 1];
            n->nr_held--;
            break;
        }
    }
    pthread_mutex_unlock(&wfg.mutex);
}

// functie interna statica
static int dfs_cycle(int curr_thread_idx, int start_thread_idx, int *visited) {
    if (visited[curr_thread_idx]) {
        return (curr_thread_idx == start_thread_idx);
    }
    visited[curr_thread_idx] = 1;
    
    int waiting_for_u = wfg.nodes[curr_thread_idx].waiting_for_user;
    if (waiting_for_u == -1) return 0;
    
    for (int i = 0; i < wfg.nr_threads; i++) {
        if (i == curr_thread_idx) continue;
        for (int j = 0; j < wfg.nodes[i].nr_held; j++) {
            if (wfg.nodes[i].held_users[j] == waiting_for_u) {
                if (dfs_cycle(i, start_thread_idx, visited)) return 1;
            }
        }
    }
    return 0;
}

int wfg_detect_deadlock(int thread_idx) {
    pthread_mutex_lock(&wfg.mutex);
    int *visited = (int *)calloc(wfg.nr_threads, sizeof(int));
    int cycle = dfs_cycle(thread_idx, thread_idx, visited);
    free(visited);
    pthread_mutex_unlock(&wfg.mutex);
    return cycle;
}

void wfg_cleanup() {
    for (int i = 0; i < wfg.nr_threads; i++) {
        free(wfg.nodes[i].held_users);
    }
    pthread_mutex_destroy(&wfg.mutex);
}
