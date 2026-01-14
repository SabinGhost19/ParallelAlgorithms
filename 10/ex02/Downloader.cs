using System.Threading.Channels;

namespace ex02
{
    public class Downloader
    {
        public async Task Run(ChannelReader<string> reader, CancellationToken token)
        {
            using HttpClient client = new HttpClient();
            try
            {
                await foreach (var url in reader.ReadAllAsync(token))
                {
                    Console.WriteLine($"Downloader: Downloading {url}");
                    byte[] data = await client.GetByteArrayAsync(url, token);
                    string fileName = Path.GetFileName(new Uri(url).LocalPath);
                    await File.WriteAllBytesAsync(fileName, data, token);
                    Console.WriteLine($"Downloader: Saved {fileName}");
                }
            }
            catch (OperationCanceledException) { }
            catch (Exception ex)
            {
                Console.WriteLine($"Downloader Error: {ex.Message}");
            }
        }
    }
}
