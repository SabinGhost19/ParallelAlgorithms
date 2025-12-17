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
