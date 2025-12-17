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
