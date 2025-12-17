# Lab 08 - Ex 02: Race Condition Fix

## Descriere Tehnica
Acest exercitiu demonstreaza rezolvarea unei conditii de cursa (race condition) clasice: incrementarea si decrementarea concurenta a unei variabile partajate.
Initial, fara sincronizare, rezultatul final era imprevizibil (diferit de 0).
Am aplicat un bloc `lock` (Monitor) in jurul operatiilor de incrementare si decrementare.
Acest lucru asigura atomicitatea operatiilor la nivel logic, garantand ca rezultatul final este corect (0) indiferent de interleaving-ul thread-urilor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Shared Variable: 0
```
