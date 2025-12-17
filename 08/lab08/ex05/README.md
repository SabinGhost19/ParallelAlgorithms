# Lab 08 - Ex 05: Parallel Prime Check (Background Thread)

## Descriere Tehnica
Similar cu exercitiul anterior, dar demonstreaza utilizarea proprietatii `IsBackground`.
Thread-urile sunt create si marcate ca `IsBackground = true`.
Un thread de fundal (background thread) nu impiedica terminarea procesului aplicatiei daca thread-ul principal (foreground) isi incheie executia.
In acest exemplu specific, folosim totusi `Join()` pentru a astepta rezultatele, dar setarea `IsBackground` este importanta pentru scenarii unde thread-urile auxiliare ar trebui sa se opreasca odata cu inchiderea aplicatiei.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
Using launch settings from ex05/Properties/launchSettings.json...
Current time: 22:04:13:0413
Thread 0: [0, 25] => 9
Thread 2: [50, 75] => 6
Thread 1: [25, 50] => 6
Thread 3: [75, 100] => 4
Elapsed Time is 14 ms
Current time: 22:04:13:0413
```
