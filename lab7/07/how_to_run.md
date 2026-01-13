# Laborator 07 - Ghid de rulare

## Compilare

```bash
# Compilare toate programele paralele
make par

# Compilare toate programele secventiale
make seq

# Sau individual:
make sample-par
make sample-seq
make getPath-par
make getPath-seq
make colorGraph-par
make colorGraph-seq
make queens-par
make queens-seq
```

## Exercitiul 1: Sample (exemplu replicated workers)

### Sintaxa
```bash
# Secvential
./sample-seq <N> <printLevel>

# Paralel
./sample-par <N> <printLevel> <P>
```
- `N` - numarul de iteratii
- `printLevel` - 0=fara output, 1=afiseaza
- `P` - numarul de thread-uri (worker)

### Test
```bash
./sample-seq 10 1
./sample-par 10 1 4
```

---

## Exercitiul 2-3: getPath (drumuri in graf)

### Sintaxa
```bash
# Secvential (afiseaza TOATE drumurile)
./getPath-seq <N> <printLevel>

# Paralel (afiseaza O SINGURA solutie)
./getPath-par <N> <printLevel> <P>
```

### Sanity Check
```bash
# Secvential - afiseaza toate drumurile de la nodul 0 la nodul 3
./getPath-seq 10 1

# Paralel - afiseaza prima solutie gasita
./getPath-par 10 1 4
```

### Rezultate asteptate
- SEQ: Multiple drumuri (ex: `0 1 2 3`, `0 4 3`, etc.)
- PAR: O singura solutie (ex: `0 4 3`)

---

## Exercitiul 4-5: colorGraph (colorarea grafurilor)

### Sintaxa
```bash
# Secvential (afiseaza TOATE colorarile)
./colorGraph-seq <N> <printLevel>

# Paralel (afiseaza O SINGURA solutie)
./colorGraph-par <N> <printLevel> <P>
```

### Sanity Check
```bash
# Secvential
./colorGraph-seq 10 1

# Paralel
./colorGraph-par 10 1 4
```

### Rezultate asteptate
- Ambele afiseaza o colorare valida (noduri adiacente au culori diferite)
- Ex: `0 1 0 1 2 1 0 2 2 1`

---

## Exercitiul 6-7: queens (problema reginelor)

### Sintaxa
```bash
# Secvential (afiseaza TOATE solutiile)
./queens-seq <N> <printLevel>

# Paralel (afiseaza O SINGURA solutie)
./queens-par <N> <printLevel> <P>
```
- `N` - dimensiunea tablei (NxN)

### Sanity Check
```bash
# Secvential N=8
./queens-seq 8 1

# Paralel N=8
./queens-par 8 1 4
```

### Rezultate asteptate
- Solutii valide pentru problema celor N regine
- Ex pentru N=8: `0 4 7 5 2 6 1 3`

---

## Masuratori de performanta

### getPath
```bash
time ./getPath-seq 10 0
time ./getPath-par 10 0 1
time ./getPath-par 10 0 4
time ./getPath-par 10 0 8
```

### colorGraph
```bash
time ./colorGraph-seq 10 0
time ./colorGraph-par 10 0 1
time ./colorGraph-par 10 0 4
time ./colorGraph-par 10 0 8
```

### queens
```bash
time ./queens-seq 10 0
time ./queens-par 10 0 4
time ./queens-par 10 0 8
```

---

## Rezultate Testare

| Program | Sanity Check | Status |
|---------|--------------|--------|
| sample-par | Afiseaza "Something X" | OK |
| getPath-par | Gaseste drum 0->3 | OK |
| colorGraph-par | Colorare valida cu 3 culori | OK |
| queens-par | Solutie valida N regine | OK |

### Note importante
- Programele paralele afiseaza O SINGURA solutie (prima gasita)
- Programele secventiale afiseaza TOATE solutiile
- Pentru N mare, folositi mai multe thread-uri pentru performanta mai buna
