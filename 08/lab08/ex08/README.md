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
