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
