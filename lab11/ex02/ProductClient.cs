using System.Net.Http.Json;

namespace ex02
{
    internal class ProductClient
    {
        private HttpClient _client = new HttpClient();
        private string _baseUrl = "http://localhost:5000/api/products";

        public async IAsyncEnumerable<Product> GetProductsAsync()
        {
            int offset = 0;
            int limit = 5;
            while (true)
            {
                List<Product>? response = null;
                try
                {
                    response = await _client.GetFromJsonAsync<List<Product>>($"{_baseUrl}?offset={offset}&limit={limit}");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error fetching products: {ex.Message}");
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
