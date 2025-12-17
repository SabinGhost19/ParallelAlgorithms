# Lab 06 - One Producer One Consumer One Buffer

## Descriere Tehnica
Acest exercitiu implementeaza problema clasica Producator-Consumator cu un singur buffer de dimensiune 1.
Sincronizarea este realizata folosind doua semafoare POSIX (`sem_t`):
- `empty`: Initializat cu 1. Indica faptul ca bufferul este gol si poate fi scris.
- `full`: Initializat cu 0. Indica faptul ca bufferul contine date si poate fi citit.

**Fluxul de executie:**
1. **Producatorul** asteapta semaforul `empty` (`sem_wait`), scrie in buffer, si semnaleaza semaforul `full` (`sem_post`).
2. **Consumatorul** asteapta semaforul `full` (`sem_wait`), citeste din buffer, si semnaleaza semaforul `empty` (`sem_post`).

Aceasta asigura ca producatorul nu suprascrie date necitite si consumatorul nu citeste date invalide.

## Executie si Output

**Comanda:**
```bash
gcc oneProducerOneConsumerOneBuffer.c -o prod_cons_1 -lpthread && ./prod_cons_1 10 2 1
```

**Standard Output:**
```
I finished Correctly
```
