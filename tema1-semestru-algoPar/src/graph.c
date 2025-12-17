#include "../headers/graph.h"

graf_t *graf = NULL;

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
    }

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
