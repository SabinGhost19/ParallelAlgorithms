using System.Collections.Concurrent;

namespace RateLimiterSystem;

public class SlidingWindowRateLimiter : IRateLimiter
{
    private readonly int _maxRequests;
    private readonly int _windowSizeSeconds;
    private readonly ConcurrentDictionary<string, NodeState> _nodeStates = new();

    private class NodeState
    {
        public long CurrentWindow;
        public int CurrentCounter;
        public int PreviousCounter;
        public readonly object Lock = new();
    }

    public SlidingWindowRateLimiter(int maxRequests, int windowSizeSeconds)
    {
        _maxRequests = maxRequests;
        _windowSizeSeconds = windowSizeSeconds;
    }

    public bool TryAcquire(string nodeId)
    {
        long currentTimestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
        long currentWindow = currentTimestamp / _windowSizeSeconds;
        double elapsedInWindow = currentTimestamp % _windowSizeSeconds;
        double percentageElapsed = elapsedInWindow / _windowSizeSeconds;

        var state = _nodeStates.GetOrAdd(nodeId, _ => new NodeState
        {
            CurrentWindow = currentWindow,
            CurrentCounter = 0,
            PreviousCounter = 0
        });

        lock (state.Lock)
        {
            if (currentWindow > state.CurrentWindow)
            {
                long diff = currentWindow - state.CurrentWindow;
                if (diff == 1)
                {
                    state.PreviousCounter = state.CurrentCounter;
                    state.CurrentCounter = 0;
                }
                else
                {
                    state.PreviousCounter = 0;
                    state.CurrentCounter = 0;
                }
                state.CurrentWindow = currentWindow;
            }

            double weightedCount = (state.PreviousCounter * (1.0 - percentageElapsed)) + state.CurrentCounter;
            if (weightedCount >= _maxRequests)
            {
                return false;
            }

            state.CurrentCounter++;
            weightedCount = (state.PreviousCounter * (1.0 - percentageElapsed)) + state.CurrentCounter;
            if (weightedCount > _maxRequests)
            {
                state.CurrentCounter--;
                return false;
            }
            return true;
        }
    }
}
