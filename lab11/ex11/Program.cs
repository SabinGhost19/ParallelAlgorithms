using System.Collections.Concurrent;

namespace ex11
{
    internal class Program
    {
        static readonly int ARRAY_SIZE = 1000000;

        static void Main(string[] args)
        {
            int[] v = new int[ARRAY_SIZE];

            init(v);
            //print(v);

            int numSections = 4;
            int sectionSize = ARRAY_SIZE / numSections;
            Action[] actions = new Action[numSections];
            ConcurrentBag<int> primes = new ConcurrentBag<int>();
            int primeCount = 0;

            for (int i = 0; i < numSections; i++)
            {
                int localStart = i * sectionSize;
                int localEnd = (i == numSections - 1) ? ARRAY_SIZE : localStart + sectionSize;
                actions[i] = () =>
                {
                    for (int j = localStart; j < localEnd; j++)
                    {
                        if (isPrime(v[j]))
                        {
                            Interlocked.Increment(ref primeCount);
                            primes.Add(v[j]);
                        }
                    }
                };
            }

            Parallel.Invoke(actions);

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