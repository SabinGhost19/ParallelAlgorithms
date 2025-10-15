# Lab 03: Race Condition

## Continut

- `racecondition`: Implementare paralela.
- `racecondition-seq`: Implementare secventiala, fara race condition.
- `testulCorrectnessINtesibe.sh`: Script pentru testarea corectitudinii si identificarea comportamentului nedeterminist.
- `image.png`: Ilustratie vizuala a conceptului de race condition si rezultat script lab.

## Imagine explicativă

![Race Condition](image.png)

## Observații

- `racecondition` poate produce rezultate diferite la fiecare rulare din cauza accesului concurent la resurse partajate.
- `racecondition-seq` ofera rezultate deterministe.
