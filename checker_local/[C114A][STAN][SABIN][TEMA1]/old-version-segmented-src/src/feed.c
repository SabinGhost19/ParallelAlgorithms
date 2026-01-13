#include "../headers/feed.h"
#include "../headers/graph.h"
#include "../headers/wfg.h"
#include "../headers/queue.h"

int similaritate(const char *s1, const char *s2) {
    int count = 0;
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int min_len = (len1 < len2) ? len1 : len2;
    for (int i = 0; i < min_len; i++) {
        if (s1[i] == s2[i]) count++;
    }
    return count;
}

void proceseaza_feed(int thread_idx, int user_id) {
    utilizator_t *u = get_user(user_id);
    if (!u) {
        notifica_task_complet(); 
        return;
    }

    int *to_lock = (int *)malloc(sizeof(int) * (u->nr_prieteni + 1));
    int count_lock = 0;
    
    to_lock[count_lock++] = user_id;
    for (int i = 0; i < u->nr_prieteni; i++) {
        to_lock[count_lock++] = u->prieteni[i];
    }
    
    int locked_count = 0;
    int deadlock_detected = 0;
    
    struct timespec ts;
    
    for (int i = 0; i < count_lock; i++) {
        int target_id = to_lock[i];
        utilizator_t *target_u = get_user(target_id);
        
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_nsec += 10000000; // 10ms
        if (ts.tv_nsec >= 1000000000) {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        
        wfg_set_waiting(thread_idx, target_id);
        int ret = pthread_mutex_timedlock(&target_u->mutex, &ts);
        wfg_set_waiting(thread_idx, -1);
        
        if (ret == 0) {
            wfg_add_held(thread_idx, target_id);
            locked_count++;
        } else {
            wfg_set_waiting(thread_idx, target_id);
            if (wfg_detect_deadlock(thread_idx)) {
                deadlock_detected = 1;
            } else {
                deadlock_detected = 1; // retry anyway on timeout
            }
            wfg_set_waiting(thread_idx, -1);
            break;
        }
    }
    
    if (deadlock_detected) {
        for (int i = 0; i < locked_count; i++) {
            int target_id = to_lock[i];
            utilizator_t *target_u = get_user(target_id);
            pthread_mutex_unlock(&target_u->mutex);
            wfg_remove_held(thread_idx, target_id);
        }
        free(to_lock);
        
        usleep(rand() % 10000);
        adauga_task(user_id); // retry
        return;
    }
    
    // calcul feed
    for (int i = 0; i < u->nr_prieteni; i++) {
        utilizator_t *friend = get_user(u->prieteni[i]);
        friend->vizitari++;
    }
    
    long long current_checksum = 0;
    
    // like-uri
    for (int i = 0; i < u->nr_prieteni; i++) {
        utilizator_t *f = get_user(u->prieteni[i]);
        for (int p = 0; p < f->nr_postari; p++) {
            current_checksum += f->postari[p].likes;
        }
    }
    
    // similaritate
    for (int i = 0; i < u->nr_prieteni; i++) {
        for (int j = i + 1; j < u->nr_prieteni; j++) {
            utilizator_t *f1 = get_user(u->prieteni[i]);
            utilizator_t *f2 = get_user(u->prieteni[j]);
            
            for (int p1 = 0; p1 < f1->nr_postari; p1++) {
                for (int p2 = 0; p2 < f2->nr_postari; p2++) {
                    current_checksum += similaritate(f1->postari[p1].content, f2->postari[p2].content);
                }
            }
        }
    }
    
    u->checksum = (u->checksum + current_checksum) % 100;
    
    for (int i = 0; i < locked_count; i++) {
        int target_id = to_lock[i];
        utilizator_t *target_u = get_user(target_id);
        pthread_mutex_unlock(&target_u->mutex);
        wfg_remove_held(thread_idx, target_id);
    }
    free(to_lock);
    
    notifica_task_complet();
}
