# Lab 10 - Ex 05: Task.WhenAny and Cancellation

## Descriere Tehnica
Acest exercitiu implementeaza un scenariu de tip "primul care termina castiga" (speculative execution).
- Se lanseaza mai multe task-uri identice care efectueaza o cautare.
- **Task.WhenAny**: Returneaza task-ul care s-a finalizat primul.
- **CancellationTokenSource**: Odata ce avem un castigator, se apeleaza `Cancel()` pentru a opri executia celorlalte task-uri care inca ruleaza, economisind resurse.
Aceasta tehnica este utila pentru a reduce latenta prin interogarea mai multor surse redundante.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
[3] working 1...
[2] working 1...
[1] working 1...
...
[2] working 10...
Final result: [2] => 12345678910
```
