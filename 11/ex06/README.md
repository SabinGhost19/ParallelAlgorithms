# Lab 11 - Ex 06: Parallel Search with External Cancellation

## Descriere Tehnica
Acest exercitiu implementeaza o cautare paralela ("find any") care se opreste imediat ce un rezultat este gasit.
- **ParallelOptions**: Se configureaza cu un `CancellationToken`.
- **Mecanism**:
    - Toate thread-urile cauta in paralel.
    - Cand un thread gaseste un numar prim, afiseaza rezultatul si apeleaza `cts.Cancel()`.
    - `Parallel.ForEach` monitorizeaza token-ul si, la detectarea anularii, inceteaza sa mai lanseze noi iteratii si incearca sa opreasca iteratiile curente (aruncand `OperationCanceledException`, care este prinsa intern sau extern).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
Found prime: 126271
...
Found prime: 752053
Search cancelled.
```
