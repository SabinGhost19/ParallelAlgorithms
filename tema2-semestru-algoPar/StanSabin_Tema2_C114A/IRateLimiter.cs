namespace RateLimiterSystem;

public interface IRateLimiter
{
    bool TryAcquire(string nodeid);
}
