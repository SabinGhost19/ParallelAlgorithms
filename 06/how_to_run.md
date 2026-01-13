# Laborator 06 - Ghid de rulare

## Compilare

```bash
# Ex1: One Producer One Consumer One Buffer
gcc -o prod_cons_1 oneProducerOneConsumerOneBuffer.c -lm -lpthread

# Ex2: One Producer One Consumer Five Buffer
gcc -o prod_cons_5 oneProducerOneConsumerFiveBuffer.c -lm -lpthread

# Ex3: Multiple Producers Multiple Consumers
gcc -o multi_prod_cons multipleProducersMultipleConsumers.c -lm -lpthread

# Ex5: Readers Writers
gcc -o readersWriters readersWriters.c -lm -lpthread

# Ex7: Philosophers
gcc -o philosophers philosophers.c -lm -lpthread

# Fake seq programs pentru script
gcc -o oneProducerOneConsumerSeq oneProducerOneConsumer-fakeForScriptSeq.c -lm -lpthread
gcc -o multipleProducersMultipleConsumersSeq multipleProducersMultipleConsumers-fakeForScriptSeq.c -lm -lpthread
gcc -o readersWritersSeq readersWriters-fakeForScriptSeq.c -lm -lpthread
gcc -o philosophersSeq philosophers-fakeForScriptSeq.c -lm -lpthread
```

## Exercitiul 1: One Producer One Consumer One Buffer

### Sintaxa
```bash
./prod_cons_1 <N> <printLevel> <P>
```
- `N` - numarul de elemente de produs/consumat
- `printLevel` - nivel de afisare (0, 1, 2)
- `P` - ignorat (folosit pentru compatibilitate)

### Test simplu
```bash
./prod_cons_1 100 1 2
# Output asteptat: I finished Correctly
```

### Test cu script
```bash
./testCorrectnessIntensive.sh oneProducerOneConsumerSeq prod_cons_1 1000 5 "2"
# Output asteptat: Output correct on intensive test
```

---

## Exercitiul 2: One Producer One Consumer Five Buffer

### Sintaxa
```bash
./prod_cons_5 <N> <printLevel> <P>
```

### Test simplu
```bash
./prod_cons_5 100 1 2
# Output asteptat: I finished Correctly
```

### Test cu script
```bash
./testCorrectnessIntensive.sh oneProducerOneConsumerSeq prod_cons_5 1000 5 "2"
# Output asteptat: Output correct on intensive test
```

---

## Exercitiul 3: Multiple Producers Multiple Consumers

### Sintaxa
```bash
./multi_prod_cons <N> <printLevel> <P>
```
- `P` - TREBUIE sa fie par si >= 4

### Test simplu
```bash
./multi_prod_cons 100 1 4
# Output asteptat: CORRECT
```

### Test cu script
```bash
./testCorrectnessIntensive.sh multipleProducersMultipleConsumersSeq multi_prod_cons 1000 5 "4 6 8"
# Output asteptat: Output correct on intensive test
```

---

## Exercitiul 5: Readers Writers

### Sintaxa
```bash
./readersWriters <N> <printLevel> <P>
```
- `P` - TREBUIE sa fie >= 2
- `N` - testat cu >= 100000

### Test simplu
```bash
./readersWriters 100000 1 2
# Output asteptat: Passed all
```

### Test cu script
```bash
./testCorrectnessIntensive.sh readersWritersSeq readersWriters 100000 3 "2 4"
# Output asteptat: Output correct on intensive test
```

---

## Exercitiul 7: Philosophers (Deadlock Resolution)

### Sintaxa
```bash
./philosophers <N> <printLevel> <P>
```
- `N` - numarul de iteratii (de cate ori mananca fiecare filozof)
- `P` - numarul de filozofi

### Test simplu (N mic din cauza sleep-ului de 1s)
```bash
./philosophers 2 1 3
# Output asteptat: All phylosophers have eaten
```

### Test cu script
```bash
./testCorrectnessIntensive.sh philosophersSeq philosophers 1 3 "3 4 5"
# Output asteptat: Output correct on intensive test
```

---

## Rezultate Testare

| Exercitiu | Test simplu | Test intensiv |
|-----------|-------------|---------------|
| Ex1 (1P1C 1Buffer) | I finished Correctly | Output correct on intensive test |
| Ex2 (1P1C 5Buffer) | I finished Correctly | Output correct on intensive test |
| Ex3 (MP MC) | CORRECT | Output correct on intensive test |
| Ex5 (Readers Writers) | Passed all | Output correct on intensive test |
| Ex7 (Philosophers) | All phylosophers have eaten | Output correct on intensive test |
