# Lab 09 - Ex 06: Thread-safe REST API

## Descriere Tehnica
Acest exercitiu vizeaza corectarea problemelor de concurenta intr-o aplicatie web ASP.NET Core existenta.
Modificarile principale au fost:
1.  **Singleton Thread-Safe**: Implementarea corecta a sablonului Singleton pentru `SingletonRepository` folosind "double-check locking" (verificare dubla a nullitatii in interiorul unui bloc `lock`) pentru a asigura ca instanta este creata o singura data chiar si in cazul accesului simultan.
2.  **Sincronizare Repository**: Adaugarea unui bloc `lock` in metoda `Add` a `OrderRepository` pentru a proteja lista de comenzi impotriva scrierilor concurente, prevenind coruperea datelor sau exceptiile de tip "Collection was modified".

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex06/ex06.csproj
```

**Standard Output:**
```
Using launch settings from ex06/Properties/launchSettings.json...
Building...
info: Microsoft.Hosting.Lifetime[14]
      Now listening on: http://localhost:5999
info: Microsoft.Hosting.Lifetime[0]
      Application started. Press Ctrl+C to shut down.
info: Microsoft.Hosting.Lifetime[0]
      Hosting environment: Development
info: Microsoft.Hosting.Lifetime[0]
      Content root path: /home/sabinghosty19/Desktop/SCHOOL/algoritmiParaleli/lab09/ex06
```
