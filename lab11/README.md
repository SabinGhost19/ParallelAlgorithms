# Lab 11 - Ex 01: Interleaving Async Work

## Descriere Tehnica
Acest exercitiu demonstreaza conceptul de "interleaving" (intercalare) a sarcinilor intr-un mediu asincron.
- **IAsyncEnumerable**: Metoda `GetNumbersAsync` produce un flux de numere asincron, simuland o operatie I/O-bound cu `await Task.Delay`.
- **Procesare Paralela**: In timp ce enumeratorul asteapta urmatorul element (`await foreach`), thread-ul nu este blocat.
- **Task.Run**: Lansam o operatie CPU-bound (`DoOtherWork`) pe un alt thread.
Observam in output cum liniile de la `DoOtherWork` se intercaleaza cu cele de la `Processing value`, demonstrand ca aplicatia ramane responsiva si utilizeaza eficient resursele in timpul asteptarilor asincrone.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
DoOtherWork(0)...
Processing value 0
DoOtherWork(1)...
Processing value 1
...
DoOtherWork(9)...
Processing value 9
Done!
```
# Lab 11 - Ex 02: Product Client with IAsyncEnumerable

## Descriere Tehnica
Acest exercitiu implementeaza un client HTTP eficient pentru consumul de date paginate.
- **HttpClient**: Este folosit pentru a face cereri GET catre un API REST (`/api/products`).
- **IAsyncEnumerable<Product>**: Metoda `GetProductsAsync` returneaza un flux asincron de produse. In loc sa descarce toate paginile si sa returneze o lista imensa (ceea ce ar consuma multa memorie si ar intarzia procesarea), metoda face `yield return` pentru fiecare produs imediat ce pagina curenta este descarcata.
- **Consum**: Clientul poate incepe procesarea primelor produse in timp ce urmatoarele pagini sunt inca pe retea.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Error fetching products: Response status code does not indicate success: 404 (Not Found).
```
*(Nota: Eroarea 404 apare deoarece endpoint-ul `/api/products` nu este implementat in backend-ul curent, dar logica clientului este corecta pentru un API functional).*
# Lab 11 - Ex 03: Search with Cancellation

## Descriere Tehnica
Acest exercitiu adauga suport pentru anulare (`Cancellation`) la clientul de produse din exercitiul anterior.
- **CancellationToken**: Este propagat de la `Main` prin `GetProductsAsync` pana la `HttpClient.GetFromJsonAsync`.
- **Logica de Cautare**: Programul cauta primele `K` produse care satisfac o conditie (pret > 50).
- **Optimizare**: Odata ce au fost gasite cele `K` produse, se apeleaza `cts.Cancel()`. Aceasta intrerupe imediat orice cerere HTTP in curs si opreste enumerarea, economisind latime de banda si timp de procesare.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
(Niciun output vizibil din cauza erorii 404 de la backend, dar comportamentul asteptat este afisarea produselor gasite si apoi "Found K products. Cancelling...")
```
# Lab 11 - Ex 05: Parallel.ForEach Prime Check

## Descriere Tehnica
Acest exercitiu utilizeaza `Parallel.ForEach` pentru a paralela o operatie CPU-intensiva (verificarea primalitatii) pe o colectie mare de date.
- **Partitionare**: `Parallel.ForEach` imparte automat colectia in partitii si le distribuie pe thread-uri din ThreadPool.
- **Sincronizare**:
    - `Interlocked.Increment`: Folosit pentru a incrementa thread-safe contorul global de numere prime. Este mult mai rapid decat un `lock` complet.
    - `ConcurrentBag<int>`: Folosit pentru a stoca numerele prime gasite. Este o structura de date optimizata pentru scrieri concurente, evitand coruperea datelor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex05/ex05.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
# Lab 11 - Ex 06: Parallel Search with External Cancellation

## Descriere Tehnica
Acest exercitiu implementeaza o cautare paralela ("find any") care se opreste imediat ce un rezultat este gasit.
- **ParallelOptions**: Se configureaza cu un `CancellationToken`.
- **Mecanism**:
    - Toate thread-urile cauta in paralel.
    - Cand un thread gaseste un numar prim, afiseaza rezultatul si apeleaza `cts.Cancel()`.
    - `Parallel.ForEach` monitorizeaza token-ul si, la detectarea anularii, inceteaza sa mai lanseze noi iteratii si incearca sa opreasca iteratiile curente (aruncand `OperationCanceledException`, care este prinsa intern sau extern).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
Found prime: 126271
...
Found prime: 752053
Search cancelled.
```
# Lab 11 - Ex 07: Parallel Search with Internal Cancellation

## Descriere Tehnica
Acest exercitiu realizeaza acelasi obiectiv ca exercitiul 6, dar folosind mecanismul nativ al buclei paralele.
- **ParallelLoopState**: Obiectul `state` este pasat delegatului executat de fiecare thread.
- **state.Stop()**: Cand un thread gaseste rezultatul, apeleaza `Stop()`. Aceasta informeaza bucla sa nu mai inceapa iteratii noi. Spre deosebire de `Break()`, `Stop()` nu garanteaza procesarea tuturor iteratiilor precedente, fiind ideal pentru cautari neordonate.
Aceasta metoda este mai eficienta decat exceptiile pentru controlul fluxului.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
Found prime: 812501
Found prime: 875011
...
Found prime: 2
```
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
# Lab 11 - Ex 09: Parallel Aggregation

## Descriere Tehnica
Acest exercitiu demonstreaza tehnica de "agregare locala" (Thread-Local Aggregation) pentru a reduce contentia pe resursele partajate.
- **Supraincarcare Parallel.ForEach**: Se foloseste varianta care accepta:
    - `localInit`: Initializeaza o variabila locala pentru fiecare thread (contorul partial).
    - `body`: Proceseaza elementele si actualizeaza contorul local (fara lock-uri).
    - `localFinally`: La finalul executiei thread-ului, adauga contorul local la cel global folosind `Interlocked.Add`.
Aceasta abordare este mult mai performanta decat incrementarea atomica a variabilei globale la fiecare iteratie.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex09/ex09.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
# Lab 11 - Ex 10: Parallel.Invoke with Array Partitioning

## Descriere Tehnica
Acest exercitiu demonstreaza procesarea paralela a unui vector prin impartirea sa in doua jumatati si procesarea fiecareia cu o metoda separata (`ProcessPartialArray`).
`Parallel.Invoke` este folosit pentru a lansa cele doua apeluri de metoda simultan.
Fiecare "task" proceseaza o gama distincta de indecsi (`[begin, end)`), eliminand necesitatea sincronizarii pentru accesul la elementele vectorului (deoarece sunt doar citite sau scrise in locatii disjuncte).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex10/ex10.csproj
```

**Standard Output:**
```
[0, 5] => 0
[5, 10] => 5
[0, 5] => 1
[5, 10] => 6
...
[5, 10] => 9
```
# Lab 11 - Ex 11: Parallel.Invoke

## Descriere Tehnica
Acest exercitiu foloseste `Parallel.Invoke` pentru a executa un set fix de actiuni in paralel.
- **Descompunere Manuala**: Vectorul este impartit explicit in 4 sectiuni egale.
- **Actiuni**: Se creeaza 4 delegati `Action`, fiecare procesand o sectiune.
- **Executie**: `Parallel.Invoke` accepta acesti delegati si ii programeaza pe ThreadPool. Executia se blocheaza pana cand toate actiunile sunt finalizate.
Aceasta metoda este utila cand avem un numar cunoscut si mic de sarcini distincte de paralelizat (task parallelism vs data parallelism).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex11/ex11.csproj
```

**Standard Output:**
```
Found 78498 primes.
```
