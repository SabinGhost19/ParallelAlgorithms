using Newtonsoft.Json;
using System.Net;
using System.Net.Http;
using System.Collections.Concurrent;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.Fonts;
using SixLabors.ImageSharp.PixelFormats;

namespace ex05
{
    // Assuming ApiResponse class is defined elsewhere or needs to be added.
    // For the purpose of this exercise, I'll assume it's available.
    // If not, it would look something like this:
    // public class ApiResponse
    // {
    //     public string Status { get; set; }
    //     public string Url { get; set; }
    // }

    internal class Program
    {
        static BlockingCollection<string> urlQueue = new BlockingCollection<string>(10);
        static BlockingCollection<(string name, byte[] data)> imageQueue = new BlockingCollection<(string, byte[])>(10);
        static int imagesToFetch = 5;

        static void Main(string[] args)
        {
            Task requester = Task.Run(Requester);
            Task downloader = Task.Run(Downloader);
            Task processer = Task.Run(Processer);

            Task.WaitAll(requester, downloader, processer);
            Console.WriteLine("All done!");
        }

        static void Requester()
        {
            using (HttpClient client = new HttpClient())
            {
                int fetched = 0;
                int backoff = 1000;

                while (fetched < imagesToFetch)
                {
                    try
                    {
                        string response = client.GetStringAsync("http://localhost:5000/api/image").Result;
                        ApiResponse? apiResponse = JsonConvert.DeserializeObject<ApiResponse>(response);

                        if (apiResponse != null && apiResponse.Status == "SUCCESS")
                        {
                            Console.WriteLine($"Requester: Got URL {apiResponse.Url}");
                            urlQueue.Add(apiResponse.Url);
                            fetched++;
                            backoff = 1000; // Reset backoff
                        }
                        else
                        {
                            Console.WriteLine($"Requester: Retry later. Backoff {backoff}ms");
                            Thread.Sleep(backoff);
                            backoff *= 2;
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Requester Error: {ex.Message}. Backoff {backoff}ms");
                        Thread.Sleep(backoff);
                        backoff *= 2;
                    }
                }
                urlQueue.CompleteAdding();
                Console.WriteLine("Requester finished.");
            }
        }

        static void Downloader()
        {
            using (WebClient webClient = new WebClient())
            {
                foreach (var url in urlQueue.GetConsumingEnumerable())
                {
                    try
                    {
                        Console.WriteLine($"Downloader: Downloading {url}");
                        byte[] data = webClient.DownloadData(url);
                        string fileName = Path.GetFileName(new Uri(url).LocalPath);
                        imageQueue.Add((fileName, data));
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Downloader Error: {ex.Message}");
                    }
                }
                imageQueue.CompleteAdding();
                Console.WriteLine("Downloader finished.");
            }
        }

        static void Processer()
        {
            FontCollection collection = new FontCollection();
            FontFamily family = collection.Add("DejaVuSans.ttf"); // Assuming we might need to provide a font or use system
            // Fallback to system fonts if possible
            if (!SystemFonts.TryGet("DejaVu Sans", out family) && !SystemFonts.TryGet("Arial", out family))
            {
                 // If no font found, we might fail drawing text. 
                 // We'll try to proceed without text or use a default if ImageSharp has one.
                 // ImageSharp doesn't have built-in fonts.
            }
            
            Font font = family.CreateFont(24, FontStyle.Bold);

            foreach (var item in imageQueue.GetConsumingEnumerable())
            {
                try
                {
                    Console.WriteLine($"Processer: Processing {item.name}");
                    using (Image image = Image.Load(item.data))
                    {
                        // Watermark
                        if (font != null)
                        {
                            image.Mutate(x => x.DrawText("WATERMARK", font, Color.Red, new PointF(10, 10)));
                        }
                        
                        string newName = Path.GetFileNameWithoutExtension(item.name) + ".watermarked" + Path.GetExtension(item.name);
                        image.Save(newName);
                        Console.WriteLine($"Processer: Saved {newName}");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Processer Error: {ex.Message}");
                }
            }
            Console.WriteLine("Processer finished.");
        }
    }
}