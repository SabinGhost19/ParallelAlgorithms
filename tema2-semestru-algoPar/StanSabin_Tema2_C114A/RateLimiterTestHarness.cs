using System.Diagnostics;
using System.Threading.Tasks;
using System.Threading;

namespace RateLimiterSystem;

public class RateLimiterTestHarness
{
    public async Task<(int approved, long timems)> TestConcurrentAccessSameNode(
        IRateLimiter limiter,
        int threadCount = 5000)
    {
        int approved = 0;
        string nodeId = "test-node-race";
        var sw = Stopwatch.StartNew();
        var tasks = new Task[threadCount];
        for (int i = 0; i < threadCount; i++)
        {
            tasks[i] = Task.Run(() =>
            {
                if (limiter.TryAcquire(nodeId))
                {
                    Interlocked.Increment(ref approved);
                }
            });
        }
        await Task.WhenAll(tasks);
        sw.Stop();
        return (approved, sw.ElapsedMilliseconds);
    }

    public async Task<(int approved, int total, long timems)> TestAccuracyUnderLoad(
        IRateLimiter limiter,
        int maxRequests = 2000)
    {
        int approved = 0;
        string nodeId = "test-node-accuracy";
        var sw = Stopwatch.StartNew();
        var tasks = new Task[maxRequests];
        for (int i = 0; i < maxRequests; i++)
        {
            tasks[i] = Task.Run(() =>
            {
                if (limiter.TryAcquire(nodeId))
                {
                    Interlocked.Increment(ref approved);
                }
            });
        }
        await Task.WhenAll(tasks);
        sw.Stop();
        return (approved, maxRequests, sw.ElapsedMilliseconds);
    }

    public async Task<(int nodes, int reqPerNode, long timems)> TestDistributedSimulation(
        IRateLimiter limiter,
        int nodeCount = 20,
        int requestsPerNode = 200)
    {
        var sw = Stopwatch.StartNew();
        var allTasks = new List<Task>();
        for (int n = 0; n < nodeCount; n++)
        {
            string nodeId = $"distributed-node-{n}";
            for (int r = 0; r < requestsPerNode; r++)
            {
                allTasks.Add(Task.Run(() =>
                {
                    limiter.TryAcquire(nodeId);
                }));
            }
        }
        await Task.WhenAll(allTasks);
        sw.Stop();
        return (nodeCount, requestsPerNode, sw.ElapsedMilliseconds);
    }
}
