# Lab 10 - Ex 07: Competing Tasks

## Descriere Tehnica
Similar cu exercitiul 5, dar aplicat pe doua task-uri distincte (`DoWork_1_Async` si `DoWork_2_Async`) care "concureaza".
Se foloseste `Task.WhenAny` pentru a determina castigatorul.
Rezultatul castigatorului este afisat, iar celalalt task este anulat proactiv prin `CancellationToken`.
Acest model este frecvent intalnit in sistemele distribuite pentru a asigura disponibilitatea si latenta scazuta (hedged requests).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
DoWork_1_Async: 2%
DoWork_2_Async: 2%
...
DoWork_1_Async: 100%
Winner: 1
```
