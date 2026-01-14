# Lab 10 - Ex 06: Processing Tasks as they Complete

## Descriere Tehnica
Acest exercitiu arata cum se pot procesa rezultatele unui set de task-uri pe masura ce acestea devin disponibile, fara a astepta finalizarea intregului set (ca la `WhenAll`).
- Se mentine o lista de task-uri active.
- Intr-o bucla `while`, se apeleaza `Task.WhenAny` pe lista curenta.
- Task-ul returnat este procesat (rezultatul afisat) si apoi eliminat din lista.
- Procesul se repeta pana cand lista devine goala.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
01
01234
0123456789
```
