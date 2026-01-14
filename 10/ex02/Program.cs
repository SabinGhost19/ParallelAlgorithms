using System.Threading.Channels;

namespace ex02
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            Channel<string> channel = Channel.CreateBounded<string>(10);
            CancellationTokenSource cts = new CancellationTokenSource();

            Requester requester = new Requester("http://localhost:5000/api/image");
            Downloader downloader = new Downloader();

            Task requesterTask = requester.Run(channel.Writer, 5, cts.Token);
            Task downloaderTask = downloader.Run(channel.Reader, cts.Token);

            await Task.WhenAll(requesterTask, downloaderTask);
            Console.WriteLine("All done!");
        }
    }
}