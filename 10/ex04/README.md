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
