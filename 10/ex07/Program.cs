namespace ex07
{
    internal class Program
    {
        private static int NUM_OF_ITERATIONS = 50;

        static async Task Main(string[] args)
        {
            try
            {
                CancellationTokenSource cts = new CancellationTokenSource();

                var progress_1 = new Progress<float>();
                progress_1.ProgressChanged += (sender, percent) =>
                {
                    Console.WriteLine($"DoWork_1_Async: {percent}%");
                };
                Task<int> t1 = DoWork_1_Async(cts.Token, progress_1);

                var progress_2 = new Progress<float>();
                progress_2.ProgressChanged += (sender, percent) =>
                {
                    Console.WriteLine($"DoWork_2_Async: {percent}%");
                };
                Task<int> t2 = DoWork_2_Async(cts.Token, progress_2);

                Task<int> winner = await Task.WhenAny(t1, t2);
                cts.Cancel();

                try {
                    Console.WriteLine($"Winner Result: {await winner}");
                } catch (Exception ex) { Console.WriteLine(ex.Message); }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        static async Task<int> DoWork_1_Async(CancellationToken cancellationToken, IProgress<float> progress = null)
        {
            int result = 0;
            Random random = new Random();

            for (int i = 1; i <= NUM_OF_ITERATIONS; i++)
            {
                progress?.Report((i * 100.0f) / NUM_OF_ITERATIONS);
                await Task.Delay(random.Next(1, 20) * 100, cancellationToken);
                result += i;
            }

            return result;
        }

        static async Task<int> DoWork_2_Async(CancellationToken cancellationToken, IProgress<float> progress = null)
        {
            int result = 0;
            Random random = new Random();

            for (int i = 1; i <= NUM_OF_ITERATIONS; i++)
            {
                progress?.Report((i * 100.0f) / NUM_OF_ITERATIONS);
                await Task.Delay(random.Next(1, 20) * 100, cancellationToken);
                result += i;
            }

            return result;
        }
    }
}