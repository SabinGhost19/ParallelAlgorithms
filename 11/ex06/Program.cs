namespace ex06
{
    internal class Program
    {
        static readonly int ARRAY_SIZE = 1000000;

        static void Main(string[] args)
        {
            int[] v = new int[ARRAY_SIZE];

            init(v);
            //print(v);

            CancellationTokenSource cts = new CancellationTokenSource();
            ParallelOptions options = new ParallelOptions { CancellationToken = cts.Token };

            try
            {
                Parallel.ForEach(v, options, (num) =>
                {
                    if (isPrime(num))
                    {
                        Console.WriteLine($"Found prime: {num}");
                        cts.Cancel();
                    }
                });
            }
            catch (OperationCanceledException)
            {
                Console.WriteLine("Search cancelled.");
            }
        }

        static bool isPrime(int n)
        {
            if (n < 2) return false;
            for (int i = 2; i * i <= n; i++)
            {
                if (n % i == 0) return false;
            }
            return true;
        }

        static void init(int[] v)
        {
            for (int i = 0; i < v.Length; i++)
            {
                v[i] = i;
            }
        }

        static void print(int[] v)
        {
            for (int i = 0; i < v.Length; i++)
            {
                Console.Write(v[i]);
                Console.Write(' ');
            }
            Console.WriteLine();
        }

        static void write(int[] v, string filename)
        {
            File.WriteAllText(filename, string.Join(" ", v));
        }
    }
}