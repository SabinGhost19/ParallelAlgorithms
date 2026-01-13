# Lab 09 - CancellationToken, AES Encryption, HTTP Clients & Thread-Safe REST API

## Cerințe generale
- .NET 9.0 SDK
- Mediu de dezvoltare: Visual Studio, VS Code, sau CLI

## Structura laboratorului

| Exercițiu | Descriere | Status |
|-----------|-----------|--------|
| ex01 | Demo CancellationToken | Complet (analiză) |
| ex02 | Căutare subșir paralel cu CancellationToken | Implementat |
| ex03 | Criptare AES multithreading | Implementat |
| ex04 | Decriptare AES multithreading | Implementat |
| ex05 | Client HTTP cu 3 thread-uri (requester, downloader, processer) | Implementat |
| ex06 | REST API thread-safe pentru comenzi online | De analizat/fixat |
| ex07 | Client multithreading pentru testare REST API (Opțional) | Implementat |

---

## Ex01 - Demo CancellationToken

### Descriere
Demonstrație a utilizării `CancellationToken` pentru anularea operațiunilor asincrone.

### Rulare
```bash
cd ex01
dotnet run
```

### Întrebări de analiză:
1. **Scopul CancellationToken**: Permite anularea cooperativă a operațiunilor pe thread-uri separate
2. **Decomentarea liniei 32**: `ThrowIfCancellationRequested()` va genera o excepție `OperationCanceledException` în loc de verificarea prin polling

---

## Ex02 - Căutare subșir paralel

### Descriere
Căutare paralelă a unui subșir într-un text mare folosind multiple thread-uri și `CancellationToken`.

### Rulare
```bash
cd ex02

# Cu 1 thread
dotnet run -- 1

# Cu 2 thread-uri
dotnet run -- 2

# Cu 4 thread-uri
dotnet run -- 4

# Cu 8 thread-uri
dotnet run -- 8
```

### Rezultate test (booksLarge.txt ~432MB):
| Thread-uri | Timp (ms) | Rezultat |
|------------|-----------|----------|
| 1 | ~38ms | Thread 0: Found at [80453097] |
| 2 | ~40ms | Thread 1: Found at [250943035] |
| 4 | ~36ms | Multiple thread-uri găsesc |
| 8 | ~32ms | Multiple thread-uri găsesc |

---

## Ex03 - Criptare AES multithreading

### Descriere
Criptare AES paralelă a unui fișier mare, împărțit în segmente procesate de thread-uri separate.

### Rulare
```bash
cd ex03

# Cu 1 thread
dotnet run -- 1

# Cu 2 thread-uri
dotnet run -- 2

# Cu 4 thread-uri
dotnet run -- 4

# Cu 8 thread-uri
dotnet run -- 8
```

### Fișiere generate:
- `books.txt_encrypted.bin` - fișierul criptat
- `books.txt_encrypted_keys.txt` - informații despre chei și intervale
- `key_N.bin`, `iv_N.bin` - cheile și vectorii de inițializare pentru fiecare segment

### Rezultate test (books.txt ~86MB):
| Thread-uri | Timp (ms) |
|------------|-----------|
| 1 | ~700ms |
| 2 | ~622ms |
| 4 | ~563ms |
| 8 | ~486ms |

---

## Ex04 - Decriptare AES multithreading

### Descriere
Decriptarea paralelă a fișierului criptat de ex03.

### Pregătire
Copiază fișierele criptate din ex03:
```bash
cd ex04
cp ../ex03/books.txt_encrypted.bin .
cp ../ex03/books.txt_encrypted_keys.txt .
cp ../ex03/key_*.bin .
cp ../ex03/iv_*.bin .
```

### Rulare
```bash
dotnet run
```

### Verificare
```bash
diff ../ex03/books.txt books.txt_decrypted.txt
# Nu ar trebui să afișeze nimic (fișierele sunt identice)
```

---

## Ex05 - Client HTTP cu 3 thread-uri

### Descriere
Client care folosește 3 thread-uri pentru:
- **Requester**: Obține URL-uri de la server cu exponential backoff
- **Downloader**: Descarcă imaginile
- **Processer**: Adaugă watermark pe imagini

### Pregătire
1. Pornește backend-ul din arhiva `backend.zip`:
```bash
cd backend/backend
dotnet run
```

2. Asigură-te că serverul rulează pe `http://localhost:5000`

### Rulare
```bash
cd ex05
dotnet run
```

### Dependențe necesare:
- SixLabors.ImageSharp
- SixLabors.ImageSharp.Drawing
- SixLabors.Fonts
- Newtonsoft.Json

---

## Ex06 - REST API thread-safe

### Descriere
Server REST API pentru procesare comenzi care trebuie făcut thread-safe.

### Problema identificată
- `ProductRepositoryBuilder` și `OrderRepositoryBuilder` au probleme de concurență
- La request-uri paralele, Singleton-ul nu este thread-safe complet

### Rulare server
```bash
cd ex06
dotnet run
```

Serverul ascultă pe `http://localhost:5999`

### Endpoints:
- `GET /product` - Listează produsele
- `POST /order` - Plasează o comandă

### Testare cu ex07 sau Postman
```bash
# GET request
curl http://localhost:5999/product

# POST request
curl -X POST http://localhost:5999/order \
  -H "Content-Type: application/json" \
  -d '[{"id":"1","quantity":1}]'
```

### Fix necesar
Problema de concurență în `ProductRepositoryBuilder.Build()` și `OrderRepositoryBuilder.Build()` care trebuie analizată și rezolvată pentru a obține Error rate = 0%.

---

## Ex07 - Client multithreading pentru testare REST API (Opțional)

### Descriere
Tool in-house pentru testarea problemelor de concurență în REST API-uri.

### Rulare
```bash
cd ex07

# Test GET cu 8 request-uri paralele
dotnet run -- -P=8 --url=http://localhost:5999/product

# Test POST cu 8 request-uri paralele
dotnet run -- -P=8 --url=http://localhost:5999/order --method=POST

# Salvare răspunsuri
dotnet run -- -P=8 --save-response

# Doar statistici specifice
dotnet run -- -P=8 --stat-countsuccess
dotnet run -- -P=8 --stat-countfail
dotnet run -- -P=8 --stat-meantime
```

### Argumente disponibile:
| Argument | Descriere |
|----------|-----------|
| `-P=N` | Număr de request-uri paralele |
| `--url=URL` | URL-ul țintă (default: http://localhost:5999/product) |
| `--method=METHOD` | Metoda HTTP: GET sau POST (default: GET) |
| `--body=JSON` | Body pentru POST requests |
| `--save-response` | Salvează răspunsurile într-un fișier JSON |
| `--stat-countsuccess` | Afișează numărul de request-uri reușite |
| `--stat-countfail` | Afișează numărul de request-uri eșuate |
| `--stat-meantime` | Afișează timpul mediu de răspuns |

### Statistici afișate (implicit):
- Successful requests
- Failed requests
- Mean response time
- Error rate
- Min/Max response time

---

## Build toate proiectele

```bash
cd lab09
dotnet build lab09.sln
```

## Note importante

1. **ex05** necesită serverul backend pornit pe portul 5000
2. **ex06** și **ex07** necesită serverul ex06 pornit pe portul 5999
3. Pentru **ex03/ex04**, asigurați-vă că fișierul `books.txt` există în directorul ex03
4. Exercițiul **ex07** este marcat ca opțional în cerințe
