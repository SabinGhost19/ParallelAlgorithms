# Lab 08 - Ex 10: ReaderWriterLockSlim

## Descriere Tehnica
Acest exercitiu implementeaza o lista thread-safe folosind `ReaderWriterLockSlim`.
Aceasta primitiva de sincronizare este optimizata pentru scenarii unde citirile sunt mult mai frecvente decat scrierile.
- `EnterReadLock()` / `ExitReadLock()`: Permite accesul simultan al mai multor thread-uri pentru citire.
- `EnterWriteLock()` / `ExitWriteLock()`: Ofera acces exclusiv unui singur thread pentru scriere, blocand atat cititorii cat si alti scriitori.
Aceasta abordare imbunatateste performanta comparativ cu un `lock` simplu care ar serializa toate accesele, inclusiv cele de citire.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex10/ex10.csproj
```

**Standard Output:**
```
READ_WRITE_CHECK: 16000 == 16000
```
