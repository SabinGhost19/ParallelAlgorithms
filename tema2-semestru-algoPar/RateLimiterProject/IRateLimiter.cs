namespace RateLimiterSystem;

/// <summary>
/// interfata comuna pentru toate implementarile de limitare a ratei.
/// </summary>
public interface IRateLimiter
{
    /// <summary>
    /// incearca sa obtina permisiunea pentru o cerere de la nodul specificat.
    /// </summary>
    /// <param name="nodeid">identificatorul nodului care face cererea.</param>
    /// <returns>true daca cererea este permisa, false daca trebuie respinsa.</returns>
    bool TryAcquire(string nodeid);
}
