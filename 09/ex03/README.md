# Lab 09 - Ex 03: AES Encryption

## Descriere Tehnica
Acest exercitiu realizeaza criptarea paralela a unui fisier text mare (`books.txt`) folosind algoritmul AES (Advanced Encryption Standard).
Fisierul este impartit logic in segmente.
Se lanseaza mai multe task-uri (`Task.Run`), fiecare responsabil de criptarea unui segment.
Pentru fiecare segment se genereaza o cheie si un vector de initializare (IV) unice.
Cheile si IV-urile sunt salvate in fisiere separate (`keys/key_{i}.txt`, `ivs/iv_{i}.txt`) pentru a permite decriptarea ulterioara.
Segmentele criptate sunt scrise concurent (sau secvential dupa join) in fisierul de iesire.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex03/ex03.csproj
```

**Standard Output:**
```
Encryption with 4 threads took 538 ms
```
