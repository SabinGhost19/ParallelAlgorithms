# Lab 11 - Ex 01: Interleaving Async Work

## Descriere Tehnica
Acest exercitiu demonstreaza conceptul de "interleaving" (intercalare) a sarcinilor intr-un mediu asincron.
- **IAsyncEnumerable**: Metoda `GetNumbersAsync` produce un flux de numere asincron, simuland o operatie I/O-bound cu `await Task.Delay`.
- **Procesare Paralela**: In timp ce enumeratorul asteapta urmatorul element (`await foreach`), thread-ul nu este blocat.
- **Task.Run**: Lansam o operatie CPU-bound (`DoOtherWork`) pe un alt thread.
Observam in output cum liniile de la `DoOtherWork` se intercaleaza cu cele de la `Processing value`, demonstrand ca aplicatia ramane responsiva si utilizeaza eficient resursele in timpul asteptarilor asincrone.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex01/ex01.csproj
```

**Standard Output:**
```
DoOtherWork(0)...
Processing value 0
DoOtherWork(1)...
Processing value 1
...
DoOtherWork(9)...
Processing value 9
Done!
```
