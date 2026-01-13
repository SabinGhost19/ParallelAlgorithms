//Stan Sabin-C114-A
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_CMD_LEN 256
#define MAX_CONTENT_LEN 8192
#define MAX_THREADS 32

typedef struct {
    int id;
    int likes;
    int length; // pre-calculated length
    char content[MAX_CONTENT_LEN];
} postare_t;

typedef struct {
    int id;
    postare_t *postari;
    int nr_postari;
    int cap_postari;
    
    int *prieteni;
    int nr_prieteni;
    int cap_prieteni;
    
    pthread_mutex_t mutex;
    int vizitari; 
    long long checksum;
} utilizator_t;

typedef struct {
    utilizator_t **utilizatori;
    int nr_utilizatori;
    int cap_utilizatori;
} graf_t;

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

graf_t *graf = NULL;
coada_taskuri_t coada;

// variabile interne pentru gestionarea terminarii
static pthread_mutex_t tasks_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tasks_cond = PTHREAD_COND_INITIALIZER;
static int tasks_remaining = 0;

// graph.c
void init_graf();
void adauga_utilizator(int id);
void adauga_postare(int user_id, char *content);
void adauga_prieten(int id1, int id2);
void adauga_like(int user_id, int post_id);
utilizator_t *get_user(int id);
void cleanup_graf();

// queue.c
void init_coada();
void adauga_task(int user_id);
int extrage_task(task_feed_t *task);
void set_queue_done();
void cleanup_coada();
void increment_pending_tasks();
void notifica_task_complet();
void wait_for_all_tasks();

// wfg.c 
void wfg_init(int n_threads) {}
void wfg_cleanup() {}

// feed.c
void proceseaza_feed(int thread_idx, int user_id);
int similaritate(postare_t *p1, postare_t *p2);

// worker.c
void *worker_function(void *arg);

// input_loader.c
int load_graph_from_file(const char *filename);
int load_tasks_from_file(const char *filename);

void init_graf() {
    graf = (graf_t *)malloc(sizeof(graf_t));
    graf->nr_utilizatori = 0;
    graf->cap_utilizatori = 10;
    graf->utilizatori = (utilizator_t **)malloc(sizeof(utilizator_t *) * graf->cap_utilizatori);
}

utilizator_t *get_user(int id) {
    if (!graf) return NULL;
    for (int i = 0; i < graf->nr_utilizatori; i++) {
        if (graf->utilizatori[i]->id == id) {
            return graf->utilizatori[i];
        }
    }
    return NULL;
}

void adauga_utilizator(int id) {
    if (get_user(id) != NULL) return;

    if (graf->nr_utilizatori == graf->cap_utilizatori) {
        graf->cap_utilizatori *= 2;
        graf->utilizatori = (utilizator_t **)realloc(graf->utilizatori, sizeof(utilizator_t *) * graf->cap_utilizatori);
    }

    utilizator_t *u = (utilizator_t *)malloc(sizeof(utilizator_t));
    u->id = id;
    u->nr_postari = 0;
    u->cap_postari = 5;
    u->postari = (postare_t *)malloc(sizeof(postare_t) * u->cap_postari);
    
    u->nr_prieteni = 0;
    u->cap_prieteni = 5;
    u->prieteni = (int *)malloc(sizeof(int) * u->cap_prieteni);
    
    pthread_mutex_init(&u->mutex, NULL);
    u->vizitari = 0;
    u->checksum = 0;

    graf->utilizatori[graf->nr_utilizatori++] = u;
}

void adauga_postare(int user_id, char *content) {
    utilizator_t *u = get_user(user_id);
    if (!u) return;

    if (u->nr_postari == u->cap_postari) {
        u->cap_postari *= 2;
        u->postari = (postare_t *)realloc(u->postari, sizeof(postare_t) * u->cap_postari);
    }

    postare_t *p = &u->postari[u->nr_postari];
    p->id = u->nr_postari; 
    p->likes = 0;
    strncpy(p->content, content, MAX_CONTENT_LEN - 1);
    p->content[MAX_CONTENT_LEN - 1] = '\0';
    
    size_t len = strlen(p->content);
    if (len > 0 && p->content[len-1] == '\n') {
        p->content[len-1] = '\0';
        len--;
    }
    p->length = len; // store length

    u->nr_postari++;
}

void adauga_prieten(int id1, int id2) {
    utilizator_t *u1 = get_user(id1);
    utilizator_t *u2 = get_user(id2);
    if (!u1 || !u2) return;

    if (u1->nr_prieteni == u1->cap_prieteni) {
        u1->cap_prieteni *= 2;
        u1->prieteni = (int *)realloc(u1->prieteni, sizeof(int) * u1->cap_prieteni);
    }
    u1->prieteni[u1->nr_prieteni++] = id2;

    if (u2->nr_prieteni == u2->cap_prieteni) {
        u2->cap_prieteni *= 2;
        u2->prieteni = (int *)realloc(u2->prieteni, sizeof(int) * u2->cap_prieteni);
    }
    u2->prieteni[u2->nr_prieteni++] = id1;
}

void adauga_like(int user_id, int post_id) {
    utilizator_t *u = get_user(user_id);
    if (!u) return;
    if (post_id >= 0 && post_id < u->nr_postari) {
        u->postari[post_id].likes++;
    }
}

void cleanup_graf() {
    if (!graf) return;
    for (int i = 0; i < graf->nr_utilizatori; i++) {
        utilizator_t *u = graf->utilizatori[i];
        free(u->postari);
        free(u->prieteni);
        pthread_mutex_destroy(&u->mutex);
        free(u);
    }
    free(graf->utilizatori);
    free(graf);
    graf = NULL;
}

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

int similaritate(postare_t *p1, postare_t *p2) {
    int count = 0;
    int min_len = (p1->length < p2->length) ? p1->length : p2->length;
    char *s1 = p1->content;
    char *s2 = p2->content;
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
    for (int i = 0; i < u->nr_prieteni; i++) {
        utilizator_t *friend = get_user(u->prieteni[i]);
        if (friend) {
            __sync_fetch_and_add(&friend->vizitari, 1);
        }
    }
    
    long long current_checksum = 0;
    
    // like-uri
    for (int i = 0; i < u->nr_prieteni; i++) {
        utilizator_t *f = get_user(u->prieteni[i]);
        if (f) {
            for (int p = 0; p < f->nr_postari; p++) {
                current_checksum += f->postari[p].likes;
            }
        }
    }
    
    // similaritate
    for (int i = 0; i < u->nr_prieteni; i++) {
        for (int j = i + 1; j < u->nr_prieteni; j++) {
            utilizator_t *f1 = get_user(u->prieteni[i]);
            utilizator_t *f2 = get_user(u->prieteni[j]);
            
            if (f1 && f2) {
                for (int p1 = 0; p1 < f1->nr_postari; p1++) {
                    for (int p2 = 0; p2 < f2->nr_postari; p2++) {
                        current_checksum += similaritate(&f1->postari[p1], &f2->postari[p2]);
                    }
                }
            }
        }
    }
    
    // update checksum - needs lock on u only
    pthread_mutex_lock(&u->mutex);
    u->checksum = (u->checksum + current_checksum) % 100;
    pthread_mutex_unlock(&u->mutex);
    
    notifica_task_complet();
}

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

int load_graph_from_file(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        perror("eroare deschidere fisier graf");
        return 0;
    }
    
    char cmd[MAX_CMD_LEN];
    while (fscanf(fin, "%s", cmd) != EOF) {
        if (strcmp(cmd, "ADD") == 0) {
            int id;
            if (fscanf(fin, "%d", &id) != 1) continue;
            adauga_utilizator(id);
        } else if (strcmp(cmd, "POST") == 0) {
            int id;
            char content[MAX_CONTENT_LEN];
            if (fscanf(fin, "%d", &id) != 1) continue;
            if (!fgets(content, MAX_CONTENT_LEN, fin)) continue;
            char *p = content;
            while (*p == ' ' || *p == '\t') p++;
            adauga_postare(id, p);
        } else if (strcmp(cmd, "FRIEND") == 0) {
            int id1, id2;
            if (fscanf(fin, "%d %d", &id1, &id2) != 2) continue;
            adauga_prieten(id1, id2);
        } else if (strcmp(cmd, "LIKE") == 0) {
            int uid, pid;
            if (fscanf(fin, "%d %d", &uid, &pid) != 2) continue;
            adauga_like(uid, pid);
        }
    }
    fclose(fin);
    return 1;
}

int load_tasks_from_file(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        perror("eroare deschidere fisier taskuri");
        return 0;
    }
    
    char cmd[MAX_CMD_LEN];
    while (fscanf(fin, "%s", cmd) != EOF) {
        if (strcmp(cmd, "FEED") == 0) {
            int id;
            if (fscanf(fin, "%d", &id) != 1) continue;
            adauga_task(id);
            increment_pending_tasks();
        }
    }
    fclose(fin);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("utilizare: %s <num_threads>\n", argv[0]);
        return 1;
    }
    
    int num_threads = atoi(argv[1]);
    if (num_threads <= 0 || num_threads > MAX_THREADS) {
        num_threads = MAX_THREADS;
    }
    
    srand(time(NULL));
    
    // initializare module
    init_graf();
    init_coada();
    wfg_init(num_threads); 
    
    // incarcare date
    if (!load_graph_from_file("in1.txt")) {
        return 1;
    }
    
    if (!load_tasks_from_file("in2.txt")) {
        return 1;
    }
    
    // pornire thread-uri
    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < num_threads; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, worker_function, arg);
    }
    
    // asteptare finalizare task-uri
    wait_for_all_tasks();
    
    // oprire worker-i
    set_queue_done();
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // afisare rezultate
    long long total_checksum = 0;
    for (int i = 0; i < graf->nr_utilizatori; i++) {
        utilizator_t *u = graf->utilizatori[i];
        printf("%d visited:%d\n", u->id, u->vizitari);
        total_checksum += u->checksum;
    }
    printf("Checksum: %lld\n", total_checksum);
    
    // curatare memorie
    cleanup_coada();
    wfg_cleanup(); 
    cleanup_graf();
    
    return 0;
}
