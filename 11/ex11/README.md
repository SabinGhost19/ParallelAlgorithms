# Lab 11 - Ex 11: Parallel.Invoke

## Descriere Tehnica
Acest exercitiu foloseste `Parallel.Invoke` pentru a executa un set fix de actiuni in paralel.
- **Descompunere Manuala**: Vectorul este impartit explicit in 4 sectiuni egale.
- **Actiuni**: Se creeaza 4 delegati `Action`, fiecare procesand o sectiune.
- **Executie**: `Parallel.Invoke` accepta acesti delegati si ii programeaza pe ThreadPool. Executia se blocheaza pana cand toate actiunile sunt finalizate.
Aceasta metoda este utila cand avem un numar cunoscut si mic de sarcini distincte de paralelizat (task parallelism vs data parallelism).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex11/ex11.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
