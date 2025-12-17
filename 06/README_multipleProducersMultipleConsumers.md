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
