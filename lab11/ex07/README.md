# Lab 11 - Ex 07: Parallel Search with Internal Cancellation

## Descriere Tehnica
Acest exercitiu realizeaza acelasi obiectiv ca exercitiul 6, dar folosind mecanismul nativ al buclei paralele.
- **ParallelLoopState**: Obiectul `state` este pasat delegatului executat de fiecare thread.
- **state.Stop()**: Cand un thread gaseste rezultatul, apeleaza `Stop()`. Aceasta informeaza bucla sa nu mai inceapa iteratii noi. Spre deosebire de `Break()`, `Stop()` nu garanteaza procesarea tuturor iteratiilor precedente, fiind ideal pentru cautari neordonate.
Aceasta metoda este mai eficienta decat exceptiile pentru controlul fluxului.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
Found prime: 812501
Found prime: 875011
...
Found prime: 2
```
