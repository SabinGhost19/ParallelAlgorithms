namespace ex03
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            double targetPrice = 129.99;
            int k = 2;
            int found = 0;
            CancellationTokenSource cts = new CancellationTokenSource();
            ProductClient client = new ProductClient();

            try
            {
                await foreach (var product in client.GetProductsAsync(cts.Token))
                {
                    if (Math.Abs(product.Price - targetPrice) < 0.01)
                    {
                        Console.WriteLine($"Found product: {product.Name} at {product.Price}");
                        found++;
                        if (found >= k)
                        {
                            Console.WriteLine("Found K products. Cancelling...");
                            cts.Cancel();
                        }
                    }
                }
            }
            catch (OperationCanceledException)
            {
                Console.WriteLine("Search cancelled.");
            }
        }
    }
}