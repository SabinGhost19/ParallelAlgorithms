using System.Collections.Concurrent;

namespace RateLimiterSystem;

/// <summary>
/// implementare token bucket pentru limitarea ratei.
/// simuleaza un "galeata" care se umple constant cu token-uri la o rata fixa.
/// fiecare cerere consuma un token; daca nu exista token-uri, cererea este respinsa.
/// algoritmul permite "burst-uri" cand galeata este plina.
/// </summary>
public class TokenBucketRateLimiter : IRateLimiter
{
    private readonly double _capacity;
    private readonly double _refillRate; // token-uri pe secunda
    private readonly ConcurrentDictionary<string, NodeState> _nodeStates = new();
    private readonly object _lockObject = new();

    /// <summary>
    /// stare pentru fiecare nod din sistemul distribuit.
    /// </summary>
    private class NodeState
    {
        public double Tokens { get; set; }
        public long LastRefillTimestamp { get; set; }
        public readonly object Lock = new();
    }

    /// <summary>
    /// initializeaza o noua instanta a token bucket.
    /// </summary>
    /// <param name="capacity">capacitatea maxima a token-urilor.</param>
    /// <param name="refillRate">numarul de token-uri adaugate pe secunda.</param>
    public TokenBucketRateLimiter(double capacity, double refillRate)
    {
        _capacity = capacity;
        _refillRate = refillRate;
    }

    /// <inheritdoc/>
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
            // calculeaza token-urile de adaugat in functie de timpul scurs
            double elapsedSeconds = (currentTimestamp - state.LastRefillTimestamp) / 1000.0;
            double tokensToAdd = elapsedSeconds * _refillRate;
            // reincarca token-urile cu protectie la depasirea capacitatii
            state.Tokens = Math.Min(_capacity, state.Tokens + tokensToAdd);
            state.LastRefillTimestamp = currentTimestamp;
            // incearca sa consume un token
            if (state.Tokens >= 1.0)
            {
                state.Tokens -= 1.0;
                return true;
            }
            return false;
        }
    }

    /// <summary>
    /// obtine timestamp-ul curent in milisecunde.
    /// </summary>
    private static long GetCurrentTimestamp()
    {
        return DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
    }
}
