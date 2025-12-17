using System.Diagnostics;

namespace ex02
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int numThreads = 1;

            string booksString = File.ReadAllText("booksLarge.txt");
            //string substring = "I need no medicine";//"This eBook is for the use of anyone anywhere";
            string substring = @"“This is Edgar’s legal nephew,” I reflected—“mine in a manner; I must
shake hands, and—yes—I must kiss him. It is right to establish a good
understanding at the beginning.”

I approached, and, attempting to take his chubby fist, said—“How do you
do, my dear?”

He replied in a jargon I did not comprehend.

“Shall you and I be friends, Hareton?” was my next essay at
conversation.

An oath, and a threat to set Throttler on me if I did not “frame off”
rewarded my perseverance.

“Hey, Throttler, lad!” whispered the little wretch, rousing a half-bred
bull-dog from its lair in a corner. “Now, wilt thou be ganging?” he
asked authoritatively.

Love for my life urged a compliance; I stepped over the threshold to
wait till the others should enter. Mr. Heathcliff was nowhere visible;
and Joseph, whom I followed to the stables, and requested to accompany
me in, after staring and muttering to himself, screwed up his nose and
replied—“Mim! mim! mim! Did iver Christian body hear aught like it?
Mincing un’ munching! How can I tell whet ye say?”";

            CancellationTokenSource cts = new CancellationTokenSource();
            
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            /////////////////////////////////////////////////////////////////////////////
            // Implement your solution here
            if (args.Length > 0) numThreads = int.Parse(args[0]);
            Thread[] threads = new Thread[numThreads];

            int len = booksString.Length;
            int chunkSize = len / numThreads;

            for (int i = 0; i < numThreads; i++)
            {
                int tid = i;
                int start = tid * chunkSize;
                // Ensure we don't go out of bounds and overlap for boundary cases
                int count = (tid == numThreads - 1) ? (len - start) : (chunkSize + substring.Length);
                if (start + count > len) count = len - start;

                threads[i] = new Thread(() => {
                    try {
                        if (cts.Token.IsCancellationRequested) return;
                        
                        // Use Ordinal for faster search
                        int index = booksString.IndexOf(substring, start, count, StringComparison.Ordinal);
                        
                        if (index != -1)
                        {
                            // Check again before printing/cancelling
                            if (!cts.Token.IsCancellationRequested)
                            {
                                Console.WriteLine($"Thread {tid}: Found at [{index}].");
                                cts.Cancel();
                            }
                        }
                    } catch (Exception) { }
                });
                threads[i].Start();
            }

            foreach (var t in threads) t.Join();
            /////////////////////////////////////////////////////////////////////////////
            
            stopwatch.Stop();
            Console.WriteLine("Elapsed time: {0}ms", stopwatch.ElapsedMilliseconds);
        }
    }
}