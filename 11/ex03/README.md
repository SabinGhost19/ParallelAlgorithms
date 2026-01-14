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
