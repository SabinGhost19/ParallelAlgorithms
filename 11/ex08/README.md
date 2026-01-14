# Lab 11 - Ex 08: Parallel Aggregation (Product)

## Descriere Tehnica
Acest exercitiu calculeaza produsul elementelor unui vector folosind `Parallel.ForEach` cu variabile locale per thread.
- **localInit**: Initializeaza produsul partial al thread-ului cu 1.
- **body**: Inmulteste elementele procesate de thread cu produsul partial local.
- **localFinally**: Inmulteste produsul partial local cu rezultatul global, protejand operatia cu un `lock`.
Aceasta metoda este mult mai eficienta decat blocarea la fiecare inmultire individuala.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex08/ex08.csproj
```

**Standard Output:**
```
120
```
