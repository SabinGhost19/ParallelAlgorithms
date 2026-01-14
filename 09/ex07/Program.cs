using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace ex07
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            // Parse arguments
            string url = "http://localhost:5999/product";
            string method = "GET";
            string? body = null;
            int parallelRequests = 1;
            bool saveResponse = false;
            bool statCountSuccess = false;
            bool statCountFail = false;
            bool statMeanTime = false;

            for (int i = 0; i < args.Length; i++)
            {
                if (args[i].StartsWith("-P="))
                {
                    parallelRequests = int.Parse(args[i].Substring(3));
                }
                else if (args[i] == "--save-response")
                {
                    saveResponse = true;
                }
                else if (args[i] == "--stat-countsuccess")
                {
                    statCountSuccess = true;
                }
                else if (args[i] == "--stat-countfail")
                {
                    statCountFail = true;
                }
                else if (args[i] == "--stat-meantime")
                {
                    statMeanTime = true;
                }
                else if (args[i].StartsWith("--url="))
                {
                    url = args[i].Substring(6);
                }
                else if (args[i].StartsWith("--method="))
                {
                    method = args[i].Substring(9).ToUpper();
                }
                else if (args[i].StartsWith("--body="))
                {
                    body = args[i].Substring(7);
                }
            }

            Console.WriteLine($"HTTP Performance Tester");
            Console.WriteLine($"=======================");
            Console.WriteLine($"URL: {url}");
            Console.WriteLine($"Method: {method}");
            Console.WriteLine($"Parallel requests: {parallelRequests}");
            Console.WriteLine();

            var results = new ConcurrentBag<RequestResult>();
            var tasks = new Task[parallelRequests];

            using var httpClient = new HttpClient();
            httpClient.Timeout = TimeSpan.FromSeconds(30);

            Stopwatch totalStopwatch = Stopwatch.StartNew();

            for (int i = 0; i < parallelRequests; i++)
            {
                int requestId = i;
                tasks[i] = Task.Run(async () =>
                {
                    var result = new RequestResult { RequestId = requestId };
                    var sw = Stopwatch.StartNew();

                    try
                    {
                        HttpResponseMessage response;

                        if (method == "POST")
                        {
                            var content = new StringContent(
                                body ?? "[{\"id\":\"1\",\"quantity\":1}]",
                                Encoding.UTF8,
                                "application/json");
                            response = await httpClient.PostAsync(url, content);
                        }
                        else
                        {
                            response = await httpClient.GetAsync(url);
                        }

                        sw.Stop();
                        result.ElapsedMs = sw.ElapsedMilliseconds;
                        result.StatusCode = (int)response.StatusCode;
                        result.Success = response.IsSuccessStatusCode;
                        result.ResponseBody = await response.Content.ReadAsStringAsync();
                    }
                    catch (Exception ex)
                    {
                        sw.Stop();
                        result.ElapsedMs = sw.ElapsedMilliseconds;
                        result.Success = false;
                        result.ErrorMessage = ex.Message;
                    }

                    results.Add(result);
                });
            }

            await Task.WhenAll(tasks);
            totalStopwatch.Stop();

            Console.WriteLine($"All requests completed in {totalStopwatch.ElapsedMilliseconds}ms");
            Console.WriteLine();

            // Save responses if requested
            if (saveResponse)
            {
                string outputFile = $"responses_{DateTime.Now:yyyyMMdd_HHmmss}.json";
                var responses = results.Select(r => new
                {
                    r.RequestId,
                    r.Success,
                    r.StatusCode,
                    r.ElapsedMs,
                    r.ResponseBody,
                    r.ErrorMessage
                }).OrderBy(r => r.RequestId);

                string json = JsonSerializer.Serialize(responses, new JsonSerializerOptions { WriteIndented = true });
                await File.WriteAllTextAsync(outputFile, json);
                Console.WriteLine($"Responses saved to: {outputFile}");
            }

            // Statistics
            Console.WriteLine("Statistics:");
            Console.WriteLine("-----------");

            int successCount = results.Count(r => r.Success);
            int failCount = results.Count(r => !r.Success);

            if (statCountSuccess || (!statCountSuccess && !statCountFail && !statMeanTime))
            {
                Console.WriteLine($"Successful requests: {successCount}");
            }

            if (statCountFail || (!statCountSuccess && !statCountFail && !statMeanTime))
            {
                Console.WriteLine($"Failed requests: {failCount}");
            }

            if (statMeanTime || (!statCountSuccess && !statCountFail && !statMeanTime))
            {
                double meanTime = results.Average(r => r.ElapsedMs);
                Console.WriteLine($"Mean response time: {meanTime:F2}ms");
            }

            if (!statCountSuccess && !statCountFail && !statMeanTime)
            {
                double errorRate = (double)failCount / parallelRequests * 100;
                Console.WriteLine($"Error rate: {errorRate:F2}%");

                if (results.Any())
                {
                    Console.WriteLine($"Min response time: {results.Min(r => r.ElapsedMs)}ms");
                    Console.WriteLine($"Max response time: {results.Max(r => r.ElapsedMs)}ms");
                }
            }
        }
    }

    class RequestResult
    {
        public int RequestId { get; set; }
        public bool Success { get; set; }
        public int StatusCode { get; set; }
        public long ElapsedMs { get; set; }
        public string? ResponseBody { get; set; }
        public string? ErrorMessage { get; set; }
    }
}
