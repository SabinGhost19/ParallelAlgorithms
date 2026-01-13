using System.Collections.Concurrent;
using System.Threading;

namespace RateLimiterSystem;

/// <summary>
/// implementare fixed window pentru limitarea ratei.
/// imparte timpul in ferestre fixe de dimensiune data si permite un numar maxim de cereri per fereastra.
/// </summary>
public class FixedWindowRateLimiter : IRateLimiter
{
    private readonly int _maxRequests;
    private readonly int _windowSizeSeconds;
    private readonly ConcurrentDictionary<string, NodeState> _nodeStates = new();

    /// <summary>
    /// stare pentru fiecare nod - contor si fereastra curenta.
    /// </summary>
    private class NodeState
    {
        public long CurrentWindow;
        public int Counter;
    }

    /// <summary>
    /// initializeaza o noua instanta a fixed window.
    /// </summary>
    /// <param name="maxRequests">numarul maxim de cereri per fereastra.</param>
    /// <param name="windowSizeSeconds">dimensiunea ferestrei in secunde.</param>
    public FixedWindowRateLimiter(int maxRequests, int windowSizeSeconds)
    {
        _maxRequests = maxRequests;
        _windowSizeSeconds = windowSizeSeconds;
    }

    /// <inheritdoc/>
    public bool TryAcquire(string nodeId)
    {
        long currentTimestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
        long currentWindow = currentTimestamp / _windowSizeSeconds;

        var state = _nodeStates.GetOrAdd(nodeId, _ => new NodeState
        {
            CurrentWindow = currentWindow,
            Counter = 0
        });

        while (true)
        {
            long stateWindow = Interlocked.Read(ref state.CurrentWindow);
            int currentCounter = Interlocked.CompareExchange(ref state.Counter, 0, 0);

            if (currentWindow > stateWindow)
            {
                if (Interlocked.CompareExchange(ref state.CurrentWindow, currentWindow, stateWindow) == stateWindow)
                {
                    Interlocked.Exchange(ref state.Counter, 0);
                    currentCounter = 0;
                }
                else
                {
                    continue;
                }
            }

            if (currentCounter >= _maxRequests)
            {
                return false;
            }

            int newCounter = Interlocked.Increment(ref state.Counter);
            if (newCounter <= _maxRequests)
            {
                return true;
            }
            else
            {
                Interlocked.Decrement(ref state.Counter);
                return false;
            }
        }
    }
}
