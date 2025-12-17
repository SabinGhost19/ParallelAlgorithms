namespace ex01
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            IAsyncEnumerable<int> numbersAsync = GetNumbersAsync();

            Task otherWork = Task.Run(() => DoOtherWork());

            await foreach (int number in numbersAsync)
            {
                Console.WriteLine($"Processing value {number}");
            }

            await otherWork;
            
            Console.ReadKey();
        }

        static async IAsyncEnumerable<int> GetNumbersAsync()
        {
            for (int i = 0; i < 10; i++)
            {
                await Task.Delay(250);
                yield return i;
            }
        }

        static void DoOtherWork()
        {
            for (int i = 0; i <= 10; i++)
            {
                Thread.Sleep(200);
                Console.WriteLine($"DoOtherWork({i})...");
            }
        }
    }
}