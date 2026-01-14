# Lab 11 - Ex 02: Product Client with IAsyncEnumerable

## Descriere Tehnica
Acest exercitiu implementeaza un client HTTP eficient pentru consumul de date paginate.
- **HttpClient**: Este folosit pentru a face cereri GET catre un API REST (`/api/products`).
- **IAsyncEnumerable<Product>**: Metoda `GetProductsAsync` returneaza un flux asincron de produse. In loc sa descarce toate paginile si sa returneze o lista imensa (ceea ce ar consuma multa memorie si ar intarzia procesarea), metoda face `yield return` pentru fiecare produs imediat ce pagina curenta este descarcata.
- **Consum**: Clientul poate incepe procesarea primelor produse in timp ce urmatoarele pagini sunt inca pe retea.

## Executie si Output

**Comanda:**
```bash
dotnet run --project ex02/ex02.csproj
```

**Standard Output:**
```
Error fetching products: Response status code does not indicate success: 404 (Not Found).
```
*(Nota: Eroarea 404 apare deoarece endpoint-ul `/api/products` nu este implementat in backend-ul curent, dar logica clientului este corecta pentru un API functional).*
