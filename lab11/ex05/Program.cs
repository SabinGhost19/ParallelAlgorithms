using System.Collections.Concurrent;

namespace ex05
{
    internal class Program
    {
        static readonly int ARRAY_SIZE = 1000000;

        static void Main(string[] args)
        {
            int[] v = new int[ARRAY_SIZE];

            init(v);
            //print(v);

            int primeCount = 0;
            ConcurrentBag<int> primes = new ConcurrentBag<int>();

            Parallel.ForEach(v, (num) =>
            {
                if (isPrime(num))
                {
                    Interlocked.Increment(ref primeCount);
                    primes.Add(num);
                }
            });

            File.WriteAllText("primes_out.txt", string.Join(" ", primes));
            File.WriteAllText("primes_out_count.txt", primeCount.ToString());
            Console.WriteLine($"Found {primeCount} primes.");
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