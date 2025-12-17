namespace ex04
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            var progress1 = new Progress<float>(percent => 
                Console.WriteLine($"[Task 1] Progress: {percent}%"));
            var progress2 = new Progress<float>(percent => 
                Console.WriteLine($"[Task 2] Progress: {percent}%"));

            Task<int> task_1 = DoSomeWorkAsync(progress1);
            Task<string> task_2 = DoSomeStringWorkAsync(progress2);
            Task task_3 = ThrowNotImplementedExceptionAsync();

            try
            {
                await Task.WhenAll(task_3, task_1, task_2);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception caught: {ex.Message}");
            }

            if (task_1.Status == TaskStatus.RanToCompletion)
                Console.WriteLine($"Task 1 Result: {task_1.Result}");
            
            if (task_2.Status == TaskStatus.RanToCompletion)
                Console.WriteLine($"Task 2 Result: {task_2.Result}");

            if (task_3.IsFaulted)
                Console.WriteLine($"Task 3 Failed: {task_3.Exception?.InnerException?.Message}");
        }

        static async Task<int> DoSomeWorkAsync(IProgress<float> progress = null)
        {
            int result = 0;
            for (int i = 1; i <= 10; i++)
            {
                // Simulate processing item i
                await Task.Delay(i * 200);

                result += Random.Shared.Next(1, 10);

                // Report progress
                progress?.Report(i * 10.0f);
            }

            return result;
        }

        static async Task<string> DoSomeStringWorkAsync(IProgress<float> progress = null)
        {
            string result = string.Empty;

            for (int i = 1; i <= 10; i++)
            {
                // Simulate processing item i
                await Task.Delay(i * 100);

                result += i;

                // Report progress
                progress?.Report(i * 10.0f);
            }

            return result;
        }

        static async Task ThrowNotImplementedExceptionAsync()
        {
            throw new NotImplementedException();
        }
    }
}