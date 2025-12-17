# Lab 06 - Philosophers

## Descriere Tehnica
Rezolvarea problemei "Cina Filozofilor" (Dining Philosophers) cu prevenirea deadlock-ului.
Solutia implementata sparge simetria cercului prin impunerea unei ordini de achizitie a furculitelor (mutex-uri):
- Filozofii cu ID impar iau intai furculita din stanga, apoi pe cea din dreapta.
- Filozofii cu ID par (sau ultimul filozof) iau intai furculita din dreapta, apoi pe cea din stanga.
Aceasta abordare garanteaza ca nu se poate forma un ciclu de asteptare (circular wait), eliminand astfel posibilitatea de deadlock.

## Executie si Output

**Comanda:**
```bash
gcc philosophers.c -o philosophers -lpthread && ./philosophers 10 2 5
```

**Standard Output:**
```
I finished Correctly
```
