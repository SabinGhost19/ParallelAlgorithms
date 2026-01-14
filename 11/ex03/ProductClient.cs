using System.Net.Http.Json;
using System.Runtime.CompilerServices;

namespace ex03
{
    internal class ProductClient
    {
        private HttpClient _client = new HttpClient();
        private string _baseUrl = "http://localhost:5000/api/products";

        public async IAsyncEnumerable<Product> GetProductsAsync([EnumeratorCancellation] CancellationToken token = default)
        {
            int offset = 0;
            int limit = 5;
            while (!token.IsCancellationRequested)
            {
                List<Product>? response = null;
                try
                {
                    response = await _client.GetFromJsonAsync<List<Product>>($"{_baseUrl}?offset={offset}&limit={limit}", token);
                }
                catch (Exception)
                {
                    yield break;
                }

                if (response == null || response.Count == 0) break;

                foreach (var p in response)
                {
                    yield return p;
                }
                offset += response.Count;
            }
        }
    }
}
