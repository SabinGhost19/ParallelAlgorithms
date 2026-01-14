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
# Lab 10 - Ex 02: Producer-Consumer with Channels

## Descriere Tehnica
Acest exercitiu implementeaza sablonul Producer-Consumer folosind `System.Threading.Channels`, o biblioteca moderna si performanta pentru comunicarea asincrona intre task-uri.
- **Canalul**: Un `Channel<string>` este creat pentru a stoca URL-urile imaginilor.
- **Producatorul (Requester)**: Scrie URL-uri in canal (`writer.WriteAsync`).
- **Consumatorul (Downloader)**: Citeste din canal (`reader.ReadAllAsync`) si proceseaza fiecare URL.
Aceasta abordare elimina necesitatea gestionarii manuale a lock-urilor si a semnalizarii, oferind un cod mai curat si mai eficient decat `BlockingCollection` in scenarii pur asincrone.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Requester: Got URL https://images.freeimages.com/images/large-previews/42b/white-cat-4-1362395.jpg
Downloader: Downloading https://images.freeimages.com/images/large-previews/42b/white-cat-4-1362395.jpg
Downloader: Saved white-cat-4-1362395.jpg
Requester: Retry later. Backoff 1000ms
Requester: Retry later. Backoff 2000ms
...
```
# Lab 10 - Ex 03: Reporting Progress

## Descriere Tehnica
Acest exercitiu demonstreaza modelul standard de raportare a progresului in operatiile asincrone folosind interfata `IProgress<T>` si clasa `Progress<T>`.
- **Progress<T>**: Captureaza contextul de sincronizare (daca exista, de exemplu in UI) si invoca callback-ul `ProgressChanged` pe acel context.
- **Raportare**: Metoda asincrona `DoSomeWorkAsync` apeleaza `progress.Report(percent)` periodic.
- **Separarea responsabilitatilor**: Logica de business nu stie cum este afisat progresul, ci doar raporteaza valori, ceea ce face codul reutilizabil si decuplat de interfata utilizator.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
BEFORE ProgressChanged => 10%
TID 5: ProgressChanged => 10%
AFTER ProgressChanged => 10%
BEFORE ProgressChanged => 20%
TID 5: ProgressChanged => 20%
...
AFTER ProgressChanged => 100%
```
# Lab 10 - Ex 04: Task.WhenAll and Progress

## Descriere Tehnica
Acest exercitiu demonstreaza coordonarea mai multor task-uri independente si raportarea progresului.
- **Task.WhenAll**: Este folosit pentru a astepta finalizarea tuturor task-urilor din lista, indiferent daca au reusit sau au esuat.
- **IProgress<T>**: Fiecare task primeste o instanta de `Progress<float>` pentru a raporta procentul de completare catre thread-ul principal, care afiseaza actualizarile in timp real.
- **Gestionarea Rezultatelor**: Dupa `WhenAll`, se inspecteaza proprietatea `Status` a fiecarui task. Daca `Status` este `RanToCompletion`, se acceseaza `Result`. Daca este `Faulted`, se gestioneaza `Exception`.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex04/ex04.csproj
```

**Standard Output:**
```
[Task 2] Progress: 10%
[Task 1] Progress: 10%
[Task 2] Progress: 20%
[Task 1] Progress: 20%
...
[Task 2] Progress: 100%
[Task 1] Progress: 100%
Exception caught: The method or operation is not implemented.
Task 1 Result: 49
Task 2 Result: 12345678910
Task 3 Failed: The method or operation is not implemented.
```
# Lab 10 - Ex 05: Task.WhenAny and Cancellation

## Descriere Tehnica
Acest exercitiu implementeaza un scenariu de tip "primul care termina castiga" (speculative execution).
- Se lanseaza mai multe task-uri identice care efectueaza o cautare.
- **Task.WhenAny**: Returneaza task-ul care s-a finalizat primul.
- **CancellationTokenSource**: Odata ce avem un castigator, se apeleaza `Cancel()` pentru a opri executia celorlalte task-uri care inca ruleaza, economisind resurse.
Aceasta tehnica este utila pentru a reduce latenta prin interogarea mai multor surse redundante.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
[3] working 1...
[2] working 1...
[1] working 1...
...
[2] working 10...
Final result: [2] => 12345678910
```
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
# Lab 10 - Ex 08: Mock Stock API Client

## Descriere Tehnica
Acest exercitiu simuleaza un agregator de preturi bursiere care interogheaza simultan mai multi furnizori de date (`ClientA`, `ClientB`).
Fiecare client are o latenta simulata aleatorie.
Folosind `Task.WhenAny`, aplicatia returneaza instantaneu primul pret primit, ignorand raspunsurile ulterioare.
Task-ul "pierzator" este anulat pentru a nu consuma resurse inutil.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex08/ex08.csproj
```

**Standard Output:**
```
Winner: ClientB: AAPL $150.05
```
