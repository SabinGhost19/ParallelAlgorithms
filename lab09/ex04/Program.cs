using System.Security.Cryptography;
using System.Text;
using System.Diagnostics;

namespace ex04
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string inputFile = "books.txt";
            string encryptedFile = inputFile + "_encrypted.bin";
            string keysFile = inputFile + "_encrypted_keys.txt";
            string decryptedFile = inputFile + "_decrypted.txt";

            if (!File.Exists(encryptedFile) || !File.Exists(keysFile))
            {
                Console.WriteLine("Encrypted files not found!");
                return;
            }

            byte[] encryptedBytes = File.ReadAllBytes(encryptedFile);
            string[] lines = File.ReadAllLines(keysFile);
            int numThreads = int.Parse(lines[0]);

            Task<byte[]>[] tasks = new Task<byte[]>[numThreads];
            
            Stopwatch sw = Stopwatch.StartNew();

            for (int i = 0; i < numThreads; i++)
            {
                string[] parts = lines[i + 1].Split(' ');
                int start = int.Parse(parts[0]);
                int end = int.Parse(parts[1]);
                string keyFile = parts[2];
                string ivFile = parts[3];

                int count = end - start;
                byte[] segment = new byte[count];
                Array.Copy(encryptedBytes, start, segment, 0, count);

                byte[] key = File.ReadAllBytes(keyFile);
                byte[] iv = File.ReadAllBytes(ivFile);

                tasks[i] = Task.Run(() => DecryptSegment(segment, key, iv));
            }

            Task.WaitAll(tasks);

            List<byte> finalDecrypted = new List<byte>();
            for (int i = 0; i < numThreads; i++)
            {
                finalDecrypted.AddRange(tasks[i].Result);
            }

            File.WriteAllBytes(decryptedFile, finalDecrypted.ToArray());
            sw.Stop();
            Console.WriteLine($"Decryption with {numThreads} threads took {sw.ElapsedMilliseconds} ms");
        }

        static byte[] DecryptSegment(byte[] input, byte[] key, byte[] iv)
        {
            using Aes aes = Aes.Create();
            aes.Key = key;
            aes.IV = iv;
            using MemoryStream inputStream = new(input);
            using CryptoStream cryptoStream = new(inputStream, aes.CreateDecryptor(), CryptoStreamMode.Read);
            using MemoryStream output = new();
            cryptoStream.CopyTo(output);
            return output.ToArray();
        }
    }
}