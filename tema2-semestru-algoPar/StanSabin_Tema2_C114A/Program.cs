using RateLimiterSystem;

await MainAsync();

async Task MainAsync()
{
    Console.WriteLine("===== sistem de limitare a ratei distribuit =====");
    Console.WriteLine($"nuclee CPU: {Environment.ProcessorCount}, runtime: .NET {Environment.Version}");
    Console.WriteLine();

    var testHarness = new RateLimiterTestHarness();
    int[] maxRequestsValues = { 100, 500, 1000 };

    await TestAlgorithm(testHarness, "Fixed Window", maxRequestsValues, maxReq => new FixedWindowRateLimiter(maxReq, windowSizeSeconds: 60));

    await TestAlgorithm(testHarness, "Sliding Window", maxRequestsValues, maxReq => new SlidingWindowRateLimiter(maxReq, windowSizeSeconds: 60));

    await TestAlgorithm(testHarness, "Token Bucket", maxRequestsValues, maxReq => new TokenBucketRateLimiter(capacity: maxReq, refillRate: maxReq / 10.0));

    Console.WriteLine("===== ALL TESTS COMPLETED =====");
}

async Task TestAlgorithm(RateLimiterTestHarness testHarness, string algorithmName, int[] maxRequestsValues, Func<int, IRateLimiter> limiterFactory)
{
    Console.WriteLine($"Algorithm: {algorithmName}");
    Console.WriteLine("---------------------------------------");
    foreach (int maxRequests in maxRequestsValues)
    {
        Console.WriteLine();
        Console.WriteLine($"MaxRequests = {maxRequests}");
        var limiter1 = limiterFactory(maxRequests);
        var (raceApproved, raceTime) = await testHarness.TestConcurrentAccessSameNode(limiter1, threadCount: 5000);
        Console.WriteLine($"RaceTest: Approved <= {raceApproved} | Time={raceTime}ms");
        var limiter2 = limiterFactory(maxRequests);
        var (accApproved, accRequests, accTime) = await testHarness.TestAccuracyUnderLoad(limiter2, maxRequests: 2000);
        Console.WriteLine($"AccuracyTest: Approved <= {accApproved} | Requests={accRequests} | Time={accTime}ms");
        var limiter3 = limiterFactory(maxRequests);
        var (nodeCount, reqPerNode, distTime) = await testHarness.TestDistributedSimulation(limiter3, nodeCount: 20, requestsPerNode: 200);
        Console.WriteLine($"DistributedTest: Nodes={nodeCount}, Req/Node={reqPerNode} | Time={distTime}ms");
    }
    Console.WriteLine();
    Console.WriteLine();
}
