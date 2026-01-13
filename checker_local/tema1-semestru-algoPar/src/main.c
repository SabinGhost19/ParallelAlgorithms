#include "../headers/common.h"
#include "../headers/graph.h"
#include "../headers/queue.h"
#include "../headers/wfg.h"
#include "../headers/worker.h"
#include "../headers/input_loader.h"

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
    if (!load_graph_from_file("in.txt")) {
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
    // accesam graful global pentru afisare
    // (in mod ideal am avea o functie de print in graph.c, dar e ok si asa)
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
