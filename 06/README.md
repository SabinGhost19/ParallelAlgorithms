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
# Lab 06 - One Producer One Consumer Five Buffer

## Descriere Tehnica
Acest exercitiu extinde problema anterioara folosind un buffer circular de dimensiune 5.
Sincronizarea utilizeaza aceleasi doua semafoare, dar cu valori initiale diferite pentru a reflecta capacitatea bufferului:
- `empty`: Initializat cu 5 (dimensiunea bufferului).
- `full`: Initializat cu 0.

Deoarece avem un singur producator si un singur consumator, nu este necesar un mutex suplimentar pentru protejarea indecsilor de scriere/citire, operatiile fiind secventializate natural de semafoare in contextul bufferului circular.

## Executie si Output

**Comanda:**
```bash
gcc oneProducerOneConsumerFiveBuffer.c -o prod_cons_5 -lpthread && ./prod_cons_5 10 2 1
```

**Standard Output:**
```
I finished Correctly
```
# Lab 06 - Multiple Producers Multiple Consumers

## Descriere Tehnica
Aceasta este varianta cea mai complexa, cu `P` producatori si `C` consumatori care acceseaza concurent un buffer circular.
Pe langa semafoarele `empty` si `full` pentru gestionarea plin/gol, sunt necesare mutex-uri (`pthread_mutex_t`) pentru a proteja indecsii bufferului (`in` si `out`) impotriva accesului simultan de catre mai multi producatori, respectiv mai multi consumatori.

**Mecanism:**
- **Producatorii** folosesc un mutex (`mutex_prod`) pentru a obtine exclusiv indexul de scriere.
- **Consumatorii** folosesc un mutex (`mutex_cons`) pentru a obtine exclusiv indexul de citire.

## Executie si Output

**Comanda:**
```bash
gcc multipleProducersMultipleConsumers.c -o multi_prod_cons -lpthread && ./multi_prod_cons 10 2 4 4
```

**Standard Output:**
```
CORRECT
```
# Lab 06 - Philosophers

## Descriere Tehnica
Rezolvarea problemei "Cina Filozofilor" (Dining Philosophers) cu prevenirea deadlock-ului.
Solutia implementata sparge simetria cercului prin impunerea unei ordini de achizitie a furculitelor (mutex-uri):
- Filozofii cu ID impar iau intai furculita din stanga, apoi pe cea din dreapta.
- Filozofii cu ID par (sau ultimul filozof) iau intai furculita din dreapta, apoi pe cea din stanga.
Aceasta abordare garanteaza ca nu se poate forma un ciclu de asteptare (circular wait), eliminand astfel posibilitatea de deadlock.

## Executie si Output

**Comanda:**
```bash
gcc philosophers.c -o philosophers -lpthread && ./philosophers 10 2 5
```

**Standard Output:**
```
I finished Correctly
```
# Lab 06 - Readers Writers

## Descriere Tehnica
Implementarea problemei Cititorilor si Scriitorilor, favorizand cititorii (multiple readers, single writer).
Sincronizarea se bazeaza pe doua mutex-uri:
1.  `mutex_readcount`: Protejeaza variabila `read_count` care numara cititorii activi.
2.  `mutex_write`: Asigura accesul exclusiv la resursa partajata.

**Logica:**
- Primul cititor care intra achizitioneaza `mutex_write`, blocand accesul scriitorilor.
- Urmatorii cititori pot intra liber (doar incrementeaza `read_count`).
- Ultimul cititor care iese elibereaza `mutex_write`, permitand accesul scriitorilor.
- Scriitorii trebuie sa obtina `mutex_write` pentru a modifica resursa.

Am adaugat `volatile` la variabilele de verificare pentru a preveni optimizarile compilatorului care ar putea masca race condition-urile in testele de corectitudine.

## Executie si Output

**Comanda:**
```bash
gcc readersWriters.c -o readersWriters -lpthread && ./readersWriters 1000 2 4
```

**Standard Output:**
```
Passed all
```
