# Lab 11 - Ex 05: Parallel.ForEach Prime Check

## Descriere Tehnica
Acest exercitiu utilizeaza `Parallel.ForEach` pentru a paralela o operatie CPU-intensiva (verificarea primalitatii) pe o colectie mare de date.
- **Partitionare**: `Parallel.ForEach` imparte automat colectia in partitii si le distribuie pe thread-uri din ThreadPool.
- **Sincronizare**:
    - `Interlocked.Increment`: Folosit pentru a incrementa thread-safe contorul global de numere prime. Este mult mai rapid decat un `lock` complet.
    - `ConcurrentBag<int>`: Folosit pentru a stoca numerele prime gasite. Este o structura de date optimizata pentru scrieri concurente, evitand coruperea datelor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
