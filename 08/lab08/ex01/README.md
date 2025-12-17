# Lab 08 - Ex 01: Thread Priorities

## Descriere Tehnica
Acest exercitiu evidentiaza impactul prioritatilor thread-urilor asupra planificarii executiei de catre sistemul de operare.
Au fost create trei thread-uri cu prioritati diferite: `Highest`, `Normal`, si `Lowest`.
Fiecare thread executa aceeasi functie (`ThreadFunction`), afisand mesaje in consola.
Desi ordinea exacta nu este garantata (fiind dependenta de OS), thread-urile cu prioritate mai mare tind sa primeasca mai mult timp de procesor si sa se execute mai rapid in conditii de incarcare.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
[14] => Thread 1 (1)
[16] => Thread 3 (1)
[15] => Thread 2 (1)
...
[14] => Thread 1 (5)
[15] => Thread 2 (5)
[16] => Thread 3 (5)
All threads have completed the work!
```
