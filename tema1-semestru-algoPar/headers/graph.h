#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

typedef struct {
    int id;
    int likes;
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

// variabila globala exportata (pentru acces rapid)
extern graf_t *graf;

void init_graf();
void adauga_utilizator(int id);
void adauga_postare(int user_id, char *content);
void adauga_prieten(int id1, int id2);
void adauga_like(int user_id, int post_id);
utilizator_t *get_user(int id);
void cleanup_graf(); // functie noua pentru curatenie

#endif // GRAPH_H
