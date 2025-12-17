Thread workerThread = new Thread(ThreadFunction);
workerThread.Start();

// Simulate some work on main thread
Thread.Sleep(2000);

workerThread.Interrupt();

workerThread.Join();

Console.WriteLine("Main program has finished.");

static void ThreadFunction()
{
    try
    {
        for (int i = 0; i < 5; i++)
        {
            Console.WriteLine($"Working... {i}");
            Thread.Sleep(100);
        }
    }
    catch (ThreadInterruptedException ex)
    {
        Console.WriteLine("Worker thread was interrupted.");
    }
    finally
    {
        Console.WriteLine("Worker thread cleanup.");
    }
}
