# Lab 10 - Ex 02: Producer-Consumer with Channels

## Descriere Tehnica
Acest exercitiu implementeaza sablonul Producer-Consumer folosind `System.Threading.Channels`, o biblioteca moderna si performanta pentru comunicarea asincrona intre task-uri.
- **Canalul**: Un `Channel<string>` este creat pentru a stoca URL-urile imaginilor.
- **Producatorul (Requester)**: Scrie URL-uri in canal (`writer.WriteAsync`).
- **Consumatorul (Downloader)**: Citeste din canal (`reader.ReadAllAsync`) si proceseaza fiecare URL.
Aceasta abordare elimina necesitatea gestionarii manuale a lock-urilor si a semnalizarii, oferind un cod mai curat si mai eficient decat `BlockingCollection` in scenarii pur asincrone.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Requester: Got URL https://images.freeimages.com/images/large-previews/42b/white-cat-4-1362395.jpg
Downloader: Downloading https://images.freeimages.com/images/large-previews/42b/white-cat-4-1362395.jpg
Downloader: Saved white-cat-4-1362395.jpg
Requester: Retry later. Backoff 1000ms
Requester: Retry later. Backoff 2000ms
...
```
