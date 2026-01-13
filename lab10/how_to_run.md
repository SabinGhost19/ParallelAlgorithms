# Lab 10 - Async/Await in C#

## Cerinte .NET
- .NET 6.0 sau mai nou

## Rulare exercitii

### Ex01 - Async/Await Basics
```bash
dotnet run --project ex01/ex01.csproj
```
**Cerinta:** Analizati comportamentul TID-urilor si decomentati linia 32 (`await task_1`) pentru a observa diferentele.

**Output asteptat:**
```
TID[1]
TID[4] => 0
TID[4] => 1
...
```

---

### Ex02 - Producer-Consumer cu Exponential Backoff
```bash
# Necesita backend-ul de la lab09/ex05 pornit pe localhost:5000
dotnet run --project ex02/ex02.csproj
```
**Cerinta:** Client `Requester` cu `HttpClient` si exponential backoff + `Downloader` pentru imagini.

**Output asteptat:**
```
Requester: Got URL https://...
Downloader: Downloading https://...
Downloader: Saved filename.jpg
```

---

### Ex03 - Progress Reporting
```bash
dotnet run --project ex03/ex03.csproj
```
**Cerinta:** Analizati si decomentati linia 28 (`await Task.Delay((i - 1) * 100)`) pentru a observa modificarea afisarii.

**Output asteptat:**
```
BEFORE ProgressChanged => 10%
TID 5: ProgressChanged => 10%
AFTER ProgressChanged => 10%
...
```

---

### Ex04 - Task.WhenAll cu Progress
```bash
dotnet run --project ex04/ex04.csproj
```
**Cerinta:** Raportare progres diferentiata per task + salvare si printare rezultate in Main().

**Output asteptat:**
```
[Task 1] Progress: 10%
[Task 2] Progress: 10%
...
Task 1 Result: 45
Task 2 Result: 12345678910
Task 3 Failed: The method or operation is not implemented.
```

---

### Ex05 - Task.WhenAny cu Cancellation
```bash
dotnet run --project ex05/ex05.csproj
```
**Cerinta:** Decomentati linia 22 (`await Task.Delay(4000)`) si observati ce se intampla. Preveniti risipa de resurse prin CancellationToken.

**Output asteptat:**
```
[1] working 1...
[2] working 1...
[3] working 1...
...
Final result: [X] => 12345678910
```

---

### Ex06 - Procesare Tasks pe Masura ce se Finalizeaza
```bash
dotnet run --project ex06/ex06.csproj
```
**Cerinta:** Procesati rezultatele pe masura ce task-urile se finalizeaza, fara a modifica ordinea din array.

**Output asteptat:**
```
01
01234
0123456789
```

---

### Ex07 - Competing Tasks
```bash
dotnet run --project ex07/ex07.csproj
```
**Cerinta:** Salvati si afisati rezultatul castigatorului, iar celalalt task sa-si intrerupa activitatea.

**Output asteptat:**
```
DoWork_1_Async: 2%
DoWork_2_Async: 2%
...
Winner Result: 1275
```

---

### Ex08 - Stock API Clients
```bash
dotnet run --project ex08/ex08.csproj
```
**Cerinta:** Interogare paralela a 2+ API-uri pentru preturi bursiere. Primul raspuns reusit este afisat, celelalte sunt anulate.

**Output asteptat:**
```
Winner: ClientA: AAPL $150.00
```
sau
```
Winner: ClientB: AAPL $150.05
```

---

## Rulare intreaga solutie
```bash
dotnet build lab10.sln
```

## Note
- Ex02 necesita backend-ul de la lab09/ex05 activ pentru a functiona complet
- Toate exercitiile demonstreaza concepte de programare asincrona: async/await, Task.WhenAll, Task.WhenAny, CancellationToken, IProgress<T>
