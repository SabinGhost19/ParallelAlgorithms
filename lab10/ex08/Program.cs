namespace ex08
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            CancellationTokenSource cts = new CancellationTokenSource();
            Task<string> t1 = ClientA.GetPriceAsync("AAPL", cts.Token);
            Task<string> t2 = ClientB.GetPriceAsync("AAPL", cts.Token);

            Task<string> winner = await Task.WhenAny(t1, t2);
            cts.Cancel();

            try {
                Console.WriteLine($"Winner: {await winner}");
            } catch (Exception ex) { Console.WriteLine(ex.Message); }
        }
    }

    class ClientA {
        public static async Task<string> GetPriceAsync(string symbol, CancellationToken token) {
            try {
                await Task.Delay(Random.Shared.Next(500, 2000), token);
                return $"ClientA: {symbol} $150.00";
            } catch (TaskCanceledException) { return "ClientA Cancelled"; }
        }
    }
    class ClientB {
        public static async Task<string> GetPriceAsync(string symbol, CancellationToken token) {
            try {
                await Task.Delay(Random.Shared.Next(500, 2000), token);
                return $"ClientB: {symbol} $150.05";
            } catch (TaskCanceledException) { return "ClientB Cancelled"; }
        }
    }
}