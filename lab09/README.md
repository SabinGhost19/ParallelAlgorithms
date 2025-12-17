# Lab 09 - Ex 01: CancellationToken

## Descriere Tehnica
Acest exercitiu exemplifica utilizarea `CancellationToken` pentru a opri controlat un task de lunga durata.
Task-ul executa o bucla infinita care simuleaza o procesare in doua faze.
La fiecare iteratie, se verifica proprietatea `IsCancellationRequested` a token-ului.
Daca token-ul a fost anulat (prin apelul `Cancel()` pe `CancellationTokenSource` in thread-ul principal), task-ul arunca o exceptie `OperationCanceledException` (sau iese gratios) si se opreste.
Acest mecanism este standardul in .NET pentru oprirea cooperativa a thread-urilor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
Press ENTER to cancel the operation.
Thread 4: working phase 1...
Thread 4: working phase 2...
...
Thread 4: working phase 1...
Thread 4: working phase 2...
Operation was cancelled.
```
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
# Lab 09 - Ex 03: AES Encryption

## Descriere Tehnica
Acest exercitiu realizeaza criptarea paralela a unui fisier text mare (`books.txt`) folosind algoritmul AES (Advanced Encryption Standard).
Fisierul este impartit logic in segmente.
Se lanseaza mai multe task-uri (`Task.Run`), fiecare responsabil de criptarea unui segment.
Pentru fiecare segment se genereaza o cheie si un vector de initializare (IV) unice.
Cheile si IV-urile sunt salvate in fisiere separate (`keys/key_{i}.txt`, `ivs/iv_{i}.txt`) pentru a permite decriptarea ulterioara.
Segmentele criptate sunt scrise concurent (sau secvential dupa join) in fisierul de iesire.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
Encryption with 4 threads took 538 ms
```
# Lab 09 - Ex 04: AES Decryption

## Descriere Tehnica
Acest exercitiu realizeaza operatia inversa a exercitiului 3: decriptarea paralela.
Programul citeste metadatele necesare (numarul de segmente, caile catre chei si IV-uri).
Se lanseaza task-uri paralele care citesc segmentele criptate corespunzatoare, le decripteaza folosind cheia si IV-ul specific, si scriu rezultatul (textul clar) intr-un buffer sau direct in fisierul destinatie.
Corectitudinea este verificata prin compararea fisierului decriptat cu cel original (de obicei folosind `diff` sau hash-uri, desi aici output-ul arata doar timpul).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex04/ex04.csproj
```

**Standard Output:**
```
Decryption with 4 threads took 461 ms
```
# Lab 09 - Ex 05: Producer-Consumer Image Processing

## Descriere Tehnica
Acest exercitiu implementeaza un pipeline complex de procesare a datelor folosind sablonul Producer-Consumer si `BlockingCollection<T>`.
Pipeline-ul este compus din 3 stadii care ruleaza in paralel:
1.  **Requester (Producator)**: Interogheaza un API REST (`http://localhost:5000/api/image`) pentru a obtine URL-uri de imagini. Implementeaza o logica de "exponential backoff" (asteptare progresiva) in cazul in care serverul returneaza erori sau nu are date.
2.  **Downloader (Procesator Intermediar)**: Preia URL-urile din coada Requester-ului, descarca imaginile de pe internet si le plaseaza in urmatoarea coada.
3.  **Processer (Consumator Final)**: Preia imaginile descarcate, le aplica un watermark (folosind biblioteca `SixLabors.ImageSharp`) si le salveaza pe disc.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
Requester: Got URL https://images.freeimages.com/images/large-previews/0fa/cat-on-books-1406468.jpg
Downloader: Downloading https://images.freeimages.com/images/large-previews/0fa/cat-on-books-1406468.jpg
Requester: Retry later. Backoff 1000ms
Requester: Retry later. Backoff 2000ms
Requester: Retry later. Backoff 4000ms
Requester: Retry later. Backoff 8000ms
Requester: Got URL https://images.freeimages.com/images/large-previews/ee6/screaming-cat-1404453.jpg
Downloader: Downloading https://images.freeimages.com/images/large-previews/ee6/screaming-cat-1404453.jpg
Requester: Retry later. Backoff 1000ms
...
```
# Lab 09 - Ex 06: Thread-safe REST API

## Descriere Tehnica
Acest exercitiu vizeaza corectarea problemelor de concurenta intr-o aplicatie web ASP.NET Core existenta.
Modificarile principale au fost:
1.  **Singleton Thread-Safe**: Implementarea corecta a sablonului Singleton pentru `SingletonRepository` folosind "double-check locking" (verificare dubla a nullitatii in interiorul unui bloc `lock`) pentru a asigura ca instanta este creata o singura data chiar si in cazul accesului simultan.
2.  **Sincronizare Repository**: Adaugarea unui bloc `lock` in metoda `Add` a `OrderRepository` pentru a proteja lista de comenzi impotriva scrierilor concurente, prevenind coruperea datelor sau exceptiile de tip "Collection was modified".

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
Using launch settings from ex06/Properties/launchSettings.json...
Building...
info: Microsoft.Hosting.Lifetime[14]
      Now listening on: http://localhost:5999
info: Microsoft.Hosting.Lifetime[0]
      Application started. Press Ctrl+C to shut down.
info: Microsoft.Hosting.Lifetime[0]
      Hosting environment: Development
info: Microsoft.Hosting.Lifetime[0]
      Content root path: /home/sabinghosty19/Desktop/SCHOOL/algoritmiParaleli/lab09/ex06
```
# Lab 09 - Ex 07: TaskCompletionSource Wrapper

## Descriere Tehnica
Acest exercitiu demonstreaza utilizarea `TaskCompletionSource<T>` pentru a transforma o operatie asincrona bazata pe callback-uri (stil "legacy") intr-una moderna, bazata pe `Task` (async/await).
- **LegacyService**: Simuleaza o componenta veche care executa o operatie pe un alt thread si notifica rezultatul prin `Action<string> onSuccess` sau `Action<Exception> onError`.
- **LegacyServiceWrapper**: Expune metoda `DoAsyncWork` care returneaza un `Task<string>`. Intern, creeaza un `TaskCompletionSource`, porneste operatia legacy, si seteaza rezultatul sau exceptia pe TCS in callback-uri.
Aceasta tehnica este esentiala pentru interoperabilitatea intre codul vechi si cel nou in .NET.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
Starting legacy operation wrapper...
[Legacy] Processing 'Test Data'...
Operation completed successfully. Result: PROCESSED: TEST DATA
Main finished.
```
