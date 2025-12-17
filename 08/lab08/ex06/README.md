# Lab 08 - Ex 06: Monitor with Try-Finally

## Descriere Tehnica
Acest exercitiu rafineaza utilizarea `Monitor` pentru a preveni deadlock-urile in cazul exceptiilor.
Codul executa o operatie riscanta (`DoSomeStuff` care poate arunca exceptii) in interiorul sectiunii critice.
Am structurat codul folosind `try-finally`:
- `Monitor.Enter` este apelat in `try`.
- `Monitor.Exit` este apelat in `finally`.
Aceasta garanteaza ca lock-ul este eliberat intotdeauna, chiar daca `DoSomeStuff` esueaza, permitand altor thread-uri sa continue executia.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
Shared Variable: 0
```
