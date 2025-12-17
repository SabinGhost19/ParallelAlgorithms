# Lab 06 - Readers Writers

## Descriere Tehnica
Implementarea problemei Cititorilor si Scriitorilor, favorizand cititorii (multiple readers, single writer).
Sincronizarea se bazeaza pe doua mutex-uri:
1.  `mutex_readcount`: Protejeaza variabila `read_count` care numara cititorii activi.
2.  `mutex_write`: Asigura accesul exclusiv la resursa partajata.

**Logica:**
- Primul cititor care intra achizitioneaza `mutex_write`, blocand accesul scriitorilor.
- Urmatorii cititori pot intra liber (doar incrementeaza `read_count`).
- Ultimul cititor care iese elibereaza `mutex_write`, permitand accesul scriitorilor.
- Scriitorii trebuie sa obtina `mutex_write` pentru a modifica resursa.

Am adaugat `volatile` la variabilele de verificare pentru a preveni optimizarile compilatorului care ar putea masca race condition-urile in testele de corectitudine.

## Executie si Output

**Comanda:**
```bash
gcc readersWriters.c -o readersWriters -lpthread && ./readersWriters 1000 2 4
```

**Standard Output:**
```
Passed all
```
