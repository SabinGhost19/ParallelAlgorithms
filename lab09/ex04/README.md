# Lab 09 - Ex 04: AES Decryption

## Descriere Tehnica
Acest exercitiu realizeaza operatia inversa a exercitiului 3: decriptarea paralela.
Programul citeste metadatele necesare (numarul de segmente, caile catre chei si IV-uri).
Se lanseaza task-uri paralele care citesc segmentele criptate corespunzatoare, le decripteaza folosind cheia si IV-ul specific, si scriu rezultatul (textul clar) intr-un buffer sau direct in fisierul destinatie.
Corectitudinea este verificata prin compararea fisierului decriptat cu cel original (de obicei folosind `diff` sau hash-uri, desi aici output-ul arata doar timpul).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex04/ex04.csproj
```

**Standard Output:**
```
Decryption with 4 threads took 461 ms
```
