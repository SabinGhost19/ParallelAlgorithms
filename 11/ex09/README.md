# Lab 11 - Ex 09: Parallel Aggregation

## Descriere Tehnica
Acest exercitiu demonstreaza tehnica de "agregare locala" (Thread-Local Aggregation) pentru a reduce contentia pe resursele partajate.
- **Supraincarcare Parallel.ForEach**: Se foloseste varianta care accepta:
    - `localInit`: Initializeaza o variabila locala pentru fiecare thread (contorul partial).
    - `body`: Proceseaza elementele si actualizeaza contorul local (fara lock-uri).
    - `localFinally`: La finalul executiei thread-ului, adauga contorul local la cel global folosind `Interlocked.Add`.
Aceasta abordare este mult mai performanta decat incrementarea atomica a variabilei globale la fiecare iteratie.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex09/ex09.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
