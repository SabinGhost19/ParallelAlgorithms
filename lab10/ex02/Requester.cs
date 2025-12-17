using System.Text.Json;
using System.Threading.Channels;

namespace ex02
{
    public class Requester
    {
        private string _targetUrl;
        private HttpClient _httpClient;

        public Requester(string targetUrl)
        {
            _targetUrl = targetUrl;
            _httpClient = new HttpClient();
        }

        public async Task Run(ChannelWriter<string> writer, int count, CancellationToken token)
        {
            int fetched = 0;
            int backoff = 1000;

            while (fetched < count && !token.IsCancellationRequested)
            {
                try
                {
                    string apiResponseString = await _httpClient.GetStringAsync(_targetUrl, token);

                    JsonSerializerOptions serializerOptions = new JsonSerializerOptions();
                    serializerOptions.PropertyNameCaseInsensitive = true;

                    ApiResponse? response = JsonSerializer.Deserialize<ApiResponse>(apiResponseString, serializerOptions);

                    if (response != null && response.Status == "SUCCESS")
                    {
                        Console.WriteLine($"Requester: Got URL {response.Url}");
                        await writer.WriteAsync(response.Url, token);
                        fetched++;
                        backoff = 1000;
                    }
                    else
                    {
                        Console.WriteLine($"Requester: Retry later. Backoff {backoff}ms");
                        await Task.Delay(backoff, token);
                        backoff *= 2;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Requester Error: {ex.Message}. Backoff {backoff}ms");
                    await Task.Delay(backoff, token);
                    backoff *= 2;
                }
            }
            writer.Complete();
            Console.WriteLine("Requester finished.");
        }
    }
}
