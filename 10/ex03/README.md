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
