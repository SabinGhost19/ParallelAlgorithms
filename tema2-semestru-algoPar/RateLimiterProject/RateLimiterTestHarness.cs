using System.Diagnostics;
using System.Threading.Tasks;
using System.Threading;

namespace RateLimiterSystem;

/// <summary>
/// suita de teste pentru verificarea algoritmilor de limitare a ratei sub incarcare concurenta variata.
/// </summary>
public class RateLimiterTestHarness
{
    /// <summary>
    /// test 1: conditie de concurenta - multiple fire pentru acelasi nod.
    /// verifica ca numarul maxim aprobat este exact (fara depasiri din race conditions).
    /// </summary>
    /// <param name="limiter">instanta rate limiter de testat.</param>
    /// <param name="threadCount">numarul de fire concurente (implicit 5000).</param>
    /// <returns>tuple cu numarul aprobat si timpul scurs in milisecunde.</returns>
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

    /// <summary>
    /// test 2: acuratete sub incarcare - genereaza cereri rapide si verifica limitele.
    /// </summary>
    /// <param name="limiter">instanta rate limiter de testat.</param>
    /// <param name="maxRequests">numarul de cereri rapide generate (implicit 2000).</param>
    /// <returns>tuple cu numarul aprobat, total cereri si timpul scurs.</returns>
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

    /// <summary>
    /// test 3 (bonus): simulare distribuita - multiple noduri cu task-uri concurente.
    /// </summary>
    /// <param name="limiter">instanta rate limiter de testat.</param>
    /// <param name="nodeCount">numarul de noduri simulate (implicit 20).</param>
    /// <param name="requestsPerNode">numarul de cereri per nod (implicit 200).</param>
    /// <returns>tuple cu numarul noduri, cereri per nod si timpul scurs.</returns>
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
