# Lab 11 - Ex 10: Parallel.Invoke with Array Partitioning

## Descriere Tehnica
Acest exercitiu demonstreaza procesarea paralela a unui vector prin impartirea sa in doua jumatati si procesarea fiecareia cu o metoda separata (`ProcessPartialArray`).
`Parallel.Invoke` este folosit pentru a lansa cele doua apeluri de metoda simultan.
Fiecare "task" proceseaza o gama distincta de indecsi (`[begin, end)`), eliminand necesitatea sincronizarii pentru accesul la elementele vectorului (deoarece sunt doar citite sau scrise in locatii disjuncte).

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex10/ex10.csproj
```

**Standard Output:**
```
[0, 5] => 0
[5, 10] => 5
[0, 5] => 1
[5, 10] => 6
...
[5, 10] => 9
```
