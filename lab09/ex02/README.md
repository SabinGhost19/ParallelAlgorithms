# Lab 09 - Ex 02: Parallel Substring Search

## Descriere Tehnica
Acest exercitiu implementeaza cautarea paralela a unui subsir intr-un text generat aleatoriu.
Se foloseste `Parallel.ForEach` pentru a itera prin posibilele pozitii de start.
Pentru a optimiza cautarea, se utilizeaza un `CancellationToken`.
Imediat ce un thread gaseste potrivirea, acesta afiseaza rezultatul si apeleaza `Cancel()` pe sursa token-ului.
Celelalte thread-uri, observand ca token-ul este anulat, isi intrerup executia, economisind astfel resurse de procesare.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Thread 0: Found at [80453097].
Elapsed time: 26ms
```
