# Lab 11 - Ex 04: Simple Parallel.ForEach

## Descriere Tehnica
Acest exercitiu este o introducere simpla in utilizarea `Parallel.ForEach` pentru paralelizarea datelor (Data Parallelism).
Se itereaza printr-un vector de numere intregi si se verifica paritatea fiecarui numar.
Deoarece operatiile sunt independente si nu modifica starea partajata (doar afiseaza la consola), nu este nevoie de mecanisme complexe de sincronizare.
Ordinea afisarii este nedeterminista, reflectand natura paralela a executiei.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex04/ex04.csproj
```

**Standard Output:**
```
74 is EVEN
76 is EVEN
...
24 is EVEN
```
