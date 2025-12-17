using System;
using System.Threading;
using System.Threading.Tasks;

namespace ex07
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            Console.WriteLine("Starting legacy operation wrapper...");

            try
            {
                string result = await LegacyServiceWrapper.DoAsyncWork("Test Data");
                Console.WriteLine($"Operation completed successfully. Result: {result}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Operation failed: {ex.Message}");
            }

            Console.WriteLine("Main finished.");
        }
    }

    // Simulates a legacy component that uses callbacks
    public class LegacyService
    {
        public void DoWork(string input, Action<string> onSuccess, Action<Exception> onError)
        {
            ThreadPool.QueueUserWorkItem(_ =>
            {
                try
                {
                    Console.WriteLine($"[Legacy] Processing '{input}'...");
                    Thread.Sleep(1000); // Simulate work

                    if (input == "error")
                    {
                        onError(new InvalidOperationException("Simulated legacy error."));
                    }
                    else
                    {
                        onSuccess($"Processed: {input.ToUpper()}");
                    }
                }
                catch (Exception ex)
                {
                    onError(ex);
                }
            });
        }
    }

    // Wraps the legacy service into a Task-based API
    public static class LegacyServiceWrapper
    {
        private static readonly LegacyService _service = new LegacyService();

        public static Task<string> DoAsyncWork(string input)
        {
            var tcs = new TaskCompletionSource<string>();

            _service.DoWork(input,
                result => tcs.SetResult(result),
                exception => tcs.SetException(exception));

            return tcs.Task;
        }
    }
}