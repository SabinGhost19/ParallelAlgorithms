# Lab 10 - Ex 01: Async/Await Basics

## Descriere Tehnica
Acest exercitiu introduce conceptele fundamentale de programare asincrona in C# folosind `async` si `await`.
- **Task.Run**: Este folosit pentru a descarca o operatie de lunga durata (simulata prin bucla si `Task.Delay`) pe un thread din ThreadPool, eliberand thread-ul principal.
- **Task.Delay**: Ofera o modalitate neblocanta de a astepta trecerea timpului, spre deosebire de `Thread.Sleep` care blocheaza thread-ul curent.
- **Fluxul de executie**: Programul demonstreaza cum thread-ul principal poate continua executia sau astepta explicit (`await`) finalizarea task-urilor asincrone.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
TID[1]
TID[1]
TID[4] => 0
TID[4] => 1
...
TID[4] => 49
TID[1]
TID[1]
```
