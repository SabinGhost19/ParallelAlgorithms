# Lab 08 - Ex 07: Monitor Synchronization

## Descriere Tehnica
Acest exercitiu ilustreaza necesitatea sincronizarii si utilizarea corecta a clasei `Monitor` (echivalentul blocului `lock` in C#).
Avem o variabila partajata care este incrementata si decrementata concurent de mai multe thread-uri.
Fara sincronizare, ar aparea race conditions.
Am utilizat `Monitor.Enter(lockObj)` pentru a obtine acces exclusiv si `Monitor.Exit(lockObj)` pentru a elibera lock-ul.
**Punct cheie:** Apelul `Monitor.Exit` este plasat intr-un bloc `finally`. Acest lucru este crucial pentru a garanta ca lock-ul este eliberat chiar daca apare o exceptie in sectiunea critica (in acest caz, o impartire la zero simulata), prevenind astfel deadlock-ul.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
Let's try do a division: 1689466359 / 1
1689466359 / 1 = 1689466359
Let's try do a division: 1229773992 / 0
Let's try do a division: 1386624747 / 1
1386624747 / 1 = 1386624747
...
Shared Variable: 0
```
