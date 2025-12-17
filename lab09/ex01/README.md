# Lab 09 - Ex 01: CancellationToken

## Descriere Tehnica
Acest exercitiu exemplifica utilizarea `CancellationToken` pentru a opri controlat un task de lunga durata.
Task-ul executa o bucla infinita care simuleaza o procesare in doua faze.
La fiecare iteratie, se verifica proprietatea `IsCancellationRequested` a token-ului.
Daca token-ul a fost anulat (prin apelul `Cancel()` pe `CancellationTokenSource` in thread-ul principal), task-ul arunca o exceptie `OperationCanceledException` (sau iese gratios) si se opreste.
Acest mecanism este standardul in .NET pentru oprirea cooperativa a thread-urilor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
Press ENTER to cancel the operation.
Thread 4: working phase 1...
Thread 4: working phase 2...
...
Thread 4: working phase 1...
Thread 4: working phase 2...
Operation was cancelled.
```
