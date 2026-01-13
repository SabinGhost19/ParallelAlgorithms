# Lab 08 - Threading in C# (.NET)

## Cerinte

### Exercitii de analiza (ex01-ex03, ex06, ex08)
Aceste exercitii sunt pentru analiza comportamentului - trebuie rulate si observate.

### Exercitii de implementare (ex04, ex05, ex07, ex09, ex10)
- **ex04**: Verificare primalitate cu threads (System.Threading.Thread)
- **ex05**: Verificare primalitate cu background threads (IsBackground = true)
- **ex07**: Rezolvare problema Monitor cu try-finally
- **ex09**: Tratare exceptie Mutex (flag mutexAcquired)
- **ex10**: Sincronizare cu ReaderWriterLockSlim

## Compilare si rulare

### Compilare toate proiectele
```bash
cd 08/lab08
dotnet build lab08.sln
```

### Rulare individuala

#### Ex01 - Analiza Thread-uri si Join
```bash
cd ex01 && dotnet run
```
**Intrebari:**
- Ce se intampla daca comentati Join-urile? Thread-ul principal se termina inainte de worker threads.
- Cand isi termina executia thread-ul principal? Dupa ce toate thread-urile au terminat (datorita Join).

#### Ex02 - Race condition si lock
```bash
cd ex02 && dotnet run
```
**Intrebari:**
- Ce valoare are sharedVariable la final? 0 (cu lock activat)
- Cu 100.000 iteratii fara lock? Valoare neprevizibila (race condition)
- Cu lock decommentat? 0 (corect sincronizat)

#### Ex03 - Thread.Interrupt
```bash
cd ex03 && dotnet run
```
**Intrebari:**
- Ce se intampla daca comentati Thread.Sleep()? Thread-ul poate termina inainte de Interrupt
- Cu 100.000 iteratii? Comportament diferit cu/fara Sleep

#### Ex04 - Verificare primalitate cu threads
```bash
cd ex04 && dotnet run <N> <P>
# Exemple:
dotnet run 100 4      # 100 numere, 4 threads
dotnet run 1000 4     # 1000 numere, 4 threads
dotnet run 10000 8    # 10000 numere, 8 threads
```
**Output:**
- Fisier `primes_in.txt` cu vectorul de input
- Fisiere `primes_<tid>_out.txt` cu numerele prime gasite de fiecare thread

#### Ex05 - Background threads
```bash
cd ex05 && dotnet run <N> <P>
# Exemple:
dotnet run 100 4
dotnet run 1000 4
```
**Diferente fata de ex04:**
- Thread-urile sunt background (IsBackground = true)
- Fara Join(), programul se termina imediat (thread-uri background sunt terminate fortat)
- Cu Join(), comportament identic cu ex04

#### Ex06 - Monitor.Enter/Exit
```bash
cd ex06 && dotnet run
```
**Comportament:**
- Monitor.Enter/Exit ofera sincronizare explicita
- DoSomeStuff() poate arunca exceptie (divide by zero)
- Finally block asigura eliberarea lock-ului

#### Ex07 - Monitor cu try-finally corect
```bash
cd ex07 && dotnet run
```
**Solutie:**
- try-finally INTERIOR pentru a garanta Monitor.Exit()
- Chiar daca DoSomeStuff() arunca exceptie, lock-ul este eliberat

#### Ex08 - Mutex inter-proces (problema)
```bash
cd ex08 && dotnet run
# In alt terminal, rulati:
cd ex08 && dotnet run
```
**Problema:**
- Daca prima instanta nu achizitioneaza mutex-ul si apeleaza ReleaseMutex(), apare exceptie
- "Object synchronization method was called from an unsynchronized block of code"

#### Ex09 - Mutex inter-proces (solutie)
```bash
cd ex09 && dotnet run
# In alt terminal, rulati:
cd ex09 && dotnet run
```
**Solutie:**
- Flag `mutexAcquired` pentru a urmari daca mutex-ul a fost achizitionat
- ReleaseMutex() apelat doar daca mutex-ul a fost achizitionat

#### Ex10 - ReaderWriterLockSlim
```bash
cd ex10 && dotnet run
```
**Implementare:**
- `_rwLock.EnterWriteLock()` / `ExitWriteLock()` pentru operatii de scriere
- `_rwLock.EnterReadLock()` / `ExitReadLock()` pentru operatii de citire
- Permite citiri simultane, dar scrierile sunt exclusive

## Sumar implementari

| Exercitiu | Status | Descriere |
|-----------|--------|-----------|
| ex01 | Analiza | Thread-uri, Join(), Priority |
| ex02 | Analiza | Race condition, lock |
| ex03 | Analiza | Thread.Interrupt |
| ex04 | Implementat | Verificare primalitate multi-threaded |
| ex05 | Implementat | Background threads |
| ex06 | Analiza | Monitor.Enter/Exit |
| ex07 | Implementat | Monitor cu try-finally corect |
| ex08 | Analiza | Mutex inter-proces (problema) |
| ex09 | Implementat | Mutex inter-proces (solutie) |
| ex10 | Implementat | ReaderWriterLockSlim |

## Concepte cheie

### Thread.Join()
- Blocheaza thread-ul curent pana cand thread-ul target termina executia
- Fara Join(), main thread poate termina inainte de worker threads

### lock vs Monitor
- `lock` este syntactic sugar pentru `Monitor.Enter()` / `Monitor.Exit()`
- Monitor ofera control mai fin (TryEnter, Wait, Pulse)
- Intern, lock foloseste try-finally cu Monitor

### Background vs Foreground Threads
- **Foreground**: Programul asteapta terminarea lor
- **Background**: Terminate fortat cand toate foreground threads termina

### Mutex
- Sincronizare inter-proces
- Poate fi numit (pentru sincronizare intre procese diferite)
- ReleaseMutex() trebuie apelat doar de thread-ul care a achizitionat mutex-ul

### ReaderWriterLockSlim
- Permite multiple citiri simultane
- Scrierile sunt exclusive (blocheaza citirile si alte scrieri)
- Mai eficient decat lock simplu pentru scenarii read-heavy
