# Lab 09 - Ex 07: TaskCompletionSource Wrapper

## Descriere Tehnica
Acest exercitiu demonstreaza utilizarea `TaskCompletionSource<T>` pentru a transforma o operatie asincrona bazata pe callback-uri (stil "legacy") intr-una moderna, bazata pe `Task` (async/await).
- **LegacyService**: Simuleaza o componenta veche care executa o operatie pe un alt thread si notifica rezultatul prin `Action<string> onSuccess` sau `Action<Exception> onError`.
- **LegacyServiceWrapper**: Expune metoda `DoAsyncWork` care returneaza un `Task<string>`. Intern, creeaza un `TaskCompletionSource`, porneste operatia legacy, si seteaza rezultatul sau exceptia pe TCS in callback-uri.
Aceasta tehnica este esentiala pentru interoperabilitatea intre codul vechi si cel nou in .NET.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex07/ex07.csproj
```

**Standard Output:**
```
Starting legacy operation wrapper...
[Legacy] Processing 'Test Data'...
Operation completed successfully. Result: PROCESSED: TEST DATA
Main finished.
```
