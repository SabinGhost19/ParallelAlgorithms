# Lab 08 - Ex 03: Thread Interrupt

## Descriere Tehnica
Acest exercitiu arata cum poate fi intrerupt un thread care se afla intr-o stare de asteptare (`Wait`, `Sleep`, `Join`).
Thread-ul secundar intra intr-o bucla de procesare cu pauze (`Thread.Sleep`).
Thread-ul principal apeleaza `workerThread.Interrupt()`.
Aceasta cauzeaza aruncarea unei exceptii `ThreadInterruptedException` in thread-ul tinta, permitandu-i sa iasa din starea de blocare si sa execute codul de curatare din blocul `catch` sau `finally`.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
Working... 0
Working... 1
...
Worker thread was interrupted.
Worker thread cleanup.
Main program has finished.
```
