# Laborator 05 - Ghid de rulare

## Compilare

```bash
gcc -o oets-par oets-par.c -lm -lpthread
gcc -o mergeSort-par mergeSort-par.c -lm -lpthread
gcc -o bubbleSort-seq bubbleSort-seq.c -lm -lpthread
```

## Exercitiul 1 & 2: OETS Paralel

### Sintaxa
```bash
./oets-par <N> <printLevel> <P>
```
- `N` - dimensiunea vectorului
- `printLevel` - 0=fara output, 1=doar verificare, 2=afiseaza vectorii
- `P` - numarul de thread-uri

### Sanity Check (N=10)
```bash
./oets-par 10 2 4
```

### Stress Test
```bash
./testCorrectnessIntensive.sh oets-par oets-par 1000 10 "1 2 4"
```

### Masuratori scalabilitate
```bash
# 1 thread
time ./oets-par 10000 0 1

# 2 thread-uri
time ./oets-par 10000 0 2

# 4 thread-uri
time ./oets-par 10000 0 4
```

## Exercitiul 3 & 4: MergeSort Paralel

### Sintaxa
```bash
./mergeSort-par <N> <printLevel> <P>
```
- `N` - dimensiunea vectorului (TREBUIE sa fie putere a lui 2)
- `printLevel` - 0=fara output, 1=doar verificare, 2=afiseaza vectorii
- `P` - numarul de thread-uri

### Sanity Check (N=32)
```bash
./mergeSort-par 32 2 4
```

### Stress Test
```bash
./testCorrectnessIntensive.sh mergeSort-par mergeSort-par 1024 10 "1 2 4"
```

### Masuratori scalabilitate
```bash
# 1 thread
time ./mergeSort-par 65536 0 1

# 2 thread-uri
time ./mergeSort-par 65536 0 2

# 4 thread-uri
time ./mergeSort-par 65536 0 4
```

## Rezultate asteptate

Ambele programe trebuie sa afiseze `Sorted correctly` la rulare cu printLevel >= 1.

Stress test-ul trebuie sa afiseze `Output correct on intensive test`.
