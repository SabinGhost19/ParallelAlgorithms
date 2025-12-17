# Lab 08 - Ex 01: Thread Priorities

## Descriere Tehnica
Acest exercitiu evidentiaza impactul prioritatilor thread-urilor asupra planificarii executiei de catre sistemul de operare.
Au fost create trei thread-uri cu prioritati diferite: `Highest`, `Normal`, si `Lowest`.
Fiecare thread executa aceeasi functie (`ThreadFunction`), afisand mesaje in consola.
Desi ordinea exacta nu este garantata (fiind dependenta de OS), thread-urile cu prioritate mai mare tind sa primeasca mai mult timp de procesor si sa se execute mai rapid in conditii de incarcare.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
[14] => Thread 1 (1)
[16] => Thread 3 (1)
[15] => Thread 2 (1)
...
[14] => Thread 1 (5)
[15] => Thread 2 (5)
[16] => Thread 3 (5)
All threads have completed the work!
```
# Lab 08 - Ex 02: Race Condition Fix

## Descriere Tehnica
Acest exercitiu demonstreaza rezolvarea unei conditii de cursa (race condition) clasice: incrementarea si decrementarea concurenta a unei variabile partajate.
Initial, fara sincronizare, rezultatul final era imprevizibil (diferit de 0).
Am aplicat un bloc `lock` (Monitor) in jurul operatiilor de incrementare si decrementare.
Acest lucru asigura atomicitatea operatiilor la nivel logic, garantand ca rezultatul final este corect (0) indiferent de interleaving-ul thread-urilor.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Shared Variable: 0
```
# Lab 08 - Ex 03: Thread Interrupt

## Descriere Tehnica
Acest exercitiu arata cum poate fi intrerupt un thread care se afla intr-o stare de asteptare (`Wait`, `Sleep`, `Join`).
Thread-ul secundar intra intr-o bucla de procesare cu pauze (`Thread.Sleep`).
Thread-ul principal apeleaza `workerThread.Interrupt()`.
Aceasta cauzeaza aruncarea unei exceptii `ThreadInterruptedException` in thread-ul tinta, permitandu-i sa iasa din starea de blocare si sa execute codul de curatare din blocul `catch` sau `finally`.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
Working... 0
Working... 1
...
Worker thread was interrupted.
Worker thread cleanup.
Main program has finished.
```
# Lab 08 - Ex 04: Parallel Prime Check (Thread)

## Descriere Tehnica
Acest exercitiu demonstreaza paralelizarea unei sarcini de calcul intensiv (verificarea numerelor prime) folosind clasa `System.Threading.Thread`.
Vectorul de numere este partitionat static in `P` segmente egale (unde `P` este numarul de procesoare logice).
Pentru fiecare segment, se instantiaza si se porneste un thread nou care executa functia `threadFunction`.
Thread-ul principal foloseste metoda `Join()` pentru a astepta terminarea tuturor thread-urilor secundare inainte de a afisa timpul total de executie.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex04/ex04.csproj
```

**Standard Output:**
```
Using launch settings from ex04/Properties/launchSettings.json...
Current time: 22:04:12:0412
Thread 0: [0, 25] => 9
Thread 2: [50, 75] => 6
Thread 1: [25, 50] => 6
Thread 3: [75, 100] => 4
Elapsed Time is 15 ms
Current time: 22:04:12:0412
```
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
# Lab 08 - Ex 08: Scanner Service with Mutex

## Descriere Tehnica
Acest exercitiu simuleaza un serviciu de scanare care trebuie sa ruleze ca o singura instanta (Singleton la nivel de OS).
Se utilizeaza clasa `ScannerService` care intern foloseste un `Mutex` numit.
Daca o alta instanta a aplicatiei ar incerca sa porneasca, nu ar putea obtine mutex-ul si s-ar opri.
Programul scaneaza o serie de fisiere simulate si scrie rezultatele intr-un log.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex08/ex08.csproj
```

**Standard Output:**
```
[ProcessID] Scanned File['0']
[ProcessID] Written results for File['0']
...
[ProcessID] Finished!
[ProcessID] Releasing the mutex...
Press any key to close the program.
```
# Lab 08 - Ex 09: Mutex Synchronization

## Descriere Tehnica
Acest exercitiu foloseste un `Mutex` (Mutual Exclusion) numit la nivel de sistem de operare.
Scopul este de a asigura ca o singura instanta a aplicatiei (sau a unei sectiuni critice inter-proces) ruleaza la un moment dat.
Spre deosebire de `Monitor` care este limitat la procesul curent (AppDomain), `Mutex`-ul poate fi folosit pentru sincronizare intre procese diferite.
Codul simuleaza scanarea unor fisiere. `WaitOne()` este folosit pentru a achizitiona mutex-ul, iar `ReleaseMutex()` pentru a-l elibera.
Am corectat logica pentru a apela `ReleaseMutex()` doar daca mutex-ul a fost obtinut cu succes, evitand exceptia `ApplicationException` ("Object synchronization method was called from an unsynchronized block of code").

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex09/ex09.csproj
```

**Standard Output:**
```
[68560] Scanned File['0']
[68560] Written results for File['0']
...
[68560] Scanned File['49']
[68560] Written results for File['49']
[68560] Finished!
[68560] Releasing the mutex...
```
# Lab 08 - Ex 10: ReaderWriterLockSlim

## Descriere Tehnica
Acest exercitiu implementeaza o lista thread-safe folosind `ReaderWriterLockSlim`.
Aceasta primitiva de sincronizare este optimizata pentru scenarii unde citirile sunt mult mai frecvente decat scrierile.
- `EnterReadLock()` / `ExitReadLock()`: Permite accesul simultan al mai multor thread-uri pentru citire.
- `EnterWriteLock()` / `ExitWriteLock()`: Ofera acces exclusiv unui singur thread pentru scriere, blocand atat cititorii cat si alti scriitori.
Aceasta abordare imbunatateste performanta comparativ cu un `lock` simplu care ar serializa toate accesele, inclusiv cele de citire.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex10/ex10.csproj
```

**Standard Output:**
```
READ_WRITE_CHECK: 16000 == 16000
```
