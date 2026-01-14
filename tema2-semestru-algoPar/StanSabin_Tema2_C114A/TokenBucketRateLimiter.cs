using System.Collections.Concurrent;

namespace RateLimiterSystem;

public class TokenBucketRateLimiter : IRateLimiter
{
    private readonly double _capacity;
    private readonly double _refillRate;
    private readonly ConcurrentDictionary<string, NodeState> _nodeStates = new();
    private readonly object _lockObject = new();

    private class NodeState
    {
        public double Tokens { get; set; }
        public long LastRefillTimestamp { get; set; }
        public readonly object Lock = new();
    }

    public TokenBucketRateLimiter(double capacity, double refillRate)
    {
        _capacity = capacity;
        _refillRate = refillRate;
    }

    public bool TryAcquire(string nodeId)
    {
        var state = _nodeStates.GetOrAdd(nodeId, _ => new NodeState
        {
            Tokens = _capacity,
            LastRefillTimestamp = GetCurrentTimestamp()
        });

        lock (state.Lock)
        {
            long currentTimestamp = GetCurrentTimestamp();
            double elapsedSeconds = (currentTimestamp - state.LastRefillTimestamp) / 1000.0;
            double tokensToAdd = elapsedSeconds * _refillRate;
            state.Tokens = Math.Min(_capacity, state.Tokens + tokensToAdd);
            state.LastRefillTimestamp = currentTimestamp;
            if (state.Tokens >= 1.0)
            {
                state.Tokens -= 1.0;
                return true;
            }
            return false;
        }
    }

    private static long GetCurrentTimestamp()
    {
        return DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
    }
}
