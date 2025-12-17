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
