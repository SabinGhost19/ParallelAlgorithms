#include "../headers/input_loader.h"
#include "../headers/graph.h"
#include "../headers/queue.h"

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
            fscanf(fin, "%d", &id);
            adauga_utilizator(id);
        } else if (strcmp(cmd, "POST") == 0) {
            int id;
            char content[MAX_CONTENT_LEN];
            fscanf(fin, "%d", &id);
            fgets(content, MAX_CONTENT_LEN, fin);
            char *p = content;
            while (*p == ' ' || *p == '\t') p++;
            adauga_postare(id, p);
        } else if (strcmp(cmd, "FRIEND") == 0) {
            int id1, id2;
            fscanf(fin, "%d %d", &id1, &id2);
            adauga_prieten(id1, id2);
        } else if (strcmp(cmd, "LIKE") == 0) {
            int uid, pid;
            fscanf(fin, "%d %d", &uid, &pid);
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
            fscanf(fin, "%d", &id);
            adauga_task(id);
            increment_pending_tasks();
        }
    }
    fclose(fin);
    return 1;
}
