# tema 1 - algoritmi paraleli

aceasta arhiva contine implementarea modulara a unei retele sociale folosind posix threads in c. proiectul pune accent pe gestionarea concurentei, sincronizarea accesului la date partajate si detectia avansata a deadlock-urilor folosind un wait-for graph.

## structura proiect

codul este organizat in module distincte:

- **headers/**: contine fisierele header (.h) cu definitiile structurilor si prototipurile functiilor.
- **src/**: contine implementarea efectiva (.c) a modulelor.
- **bin/**: directorul unde se genereaza executabilul.
- **obj/**: fisierele obiect intermediare.

## module principale

- **graph**: gestioneaza structurile de date (utilizatori, postari, prietenii). ofera functii thread-safe pentru modificarea grafului.
- **wfg (wait-for graph)**: implementeaza mecanismul de detectie a deadlock-urilor. mentine un graf orientat al dependentelor dintre thread-uri si resurse. detectia ciclurilor se face printr-o parcurgere dfs (depth-first search).
- **queue**: implementeaza o coada thread-safe (producator-consumator) pentru distribuirea task-urilor de calcul feed catre thread-urile worker.
- **feed**: contine logica de business pentru calculul feed-ului si algoritmul de similaritate a textelor.
- **worker**: defineste comportamentul thread-urilor care proceseaza task-uri in paralel.
- **input_loader**: se ocupa de parsarea fisierelor de intrare.

## compilare

proiectul foloseste un `makefile` pentru automatizarea procesului de compilare.

pentru a compila tot proiectul:
```bash
make
```

pentru a sterge fisierele generate (curatenie):
```bash
make clean
```

## rulare

executabilul este generat in directorul `bin`. se ruleaza specificand numarul de thread-uri:

```bash
./bin/main <numar_threaduri>
```

exemplu:
```bash
./bin/main 4
```

## detalii tehnice

### sincronizare si deadlock
fiecare utilizator are asociat un mutex propriu. pentru a calcula feed-ul unui utilizator, un thread trebuie sa blocheze simultan utilizatorul respectiv si toti prietenii acestuia.

pentru a evita blocajele (deadlock), se foloseste urmatoarea strategie:
1. se incearca blocarea resurselor folosind `pthread_mutex_timedlock` cu un timeout de 10ms.
2. daca timeout-ul expira, se invoca mecanismul de detectie deadlock din modulul `wfg`.
3. daca se detecteaza un ciclu in graful de asteptare, thread-ul elibereaza toate resursele detinute, asteapta un timp aleator (backoff) si repune task-ul in coada pentru a fi procesat ulterior.

### calcul feed
feed-ul este calculat prin insumarea like-urilor si a gradului de similaritate intre postarile prietenilor. similaritatea se determina prin compararea caracter cu caracter a continutului postarilor.

## intrebari frecvente (faq)

**1. ce se intampla daca nu pot obtine toate resursele?**
se foloseste mecanismul de timeout. daca expira, se verifica existenta deadlock-ului. daca exista, se elibereaza tot si se reincearca dupa un timp.

**2. de ce interval aleatoriu (backoff)?**
pentru a preveni livelock-ul, adica situatia in care thread-urile reincearca simultan si se blocheaza reciproc la infinit.

**3. cum se detecteaza ciclurile?**
folosind dfs (depth-first search) cu stiva de recursie. daca ajungem la un nod deja vizitat in stiva curenta, avem un ciclu.

**4. cum verific race conditions?**
se poate folosi utilitarul helgrind:
```bash
valgrind --tool=helgrind ./bin/main 8
```

## fisiere intrare
- `in.txt`: contine comenzile pentru constructia initiala a grafului (add, post, friend, like). este procesat secvential.
- `in2.txt`: contine comenzile de calcul feed. acestea sunt adaugate in coada si procesate paralel.
