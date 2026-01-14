namespace ex02
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            ProductClient client = new ProductClient();
            await foreach (var product in client.GetProductsAsync())
            {
                Console.WriteLine($"Processing product [{product.Id}]: Name={product.Name}, Category={product.Category}, Description={product.Description}, Price={product.Price}");
            }
        }
    }
}