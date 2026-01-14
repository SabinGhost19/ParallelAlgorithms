# Lab 10 - Ex 08: Mock Stock API Client

## Descriere Tehnica
Acest exercitiu simuleaza un agregator de preturi bursiere care interogheaza simultan mai multi furnizori de date (`ClientA`, `ClientB`).
Fiecare client are o latenta simulata aleatorie.
Folosind `Task.WhenAny`, aplicatia returneaza instantaneu primul pret primit, ignorand raspunsurile ulterioare.
Task-ul "pierzator" este anulat pentru a nu consuma resurse inutil.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex08/ex08.csproj
```

**Standard Output:**
```
Winner: ClientB: AAPL $150.05
```
