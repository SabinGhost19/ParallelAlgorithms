using System.Security.Cryptography;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace ex03
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string inputFile = "books.txt";
            string outputEncryptedFile = inputFile + "_encrypted.bin";
            string outputKeysFile = inputFile + "_encrypted_keys.txt";

            if (!File.Exists(inputFile))
            {
                Console.WriteLine($"File {inputFile} not found!");
                return;
            }

            int numThreads = args.Length > 0 ? int.Parse(args[0]) : 4;
            byte[] inputBytes = File.ReadAllBytes(inputFile);
            int len = inputBytes.Length;
            int chunkSize = len / numThreads;

            Task<byte[]>[] tasks = new Task<byte[]>[numThreads];
            string[] keyFiles = new string[numThreads];
            string[] ivFiles = new string[numThreads];

            Stopwatch sw = Stopwatch.StartNew();

            for (int i = 0; i < numThreads; i++)
            {
                int tid = i;
                int start = tid * chunkSize;
                int count = (tid == numThreads - 1) ? (len - start) : chunkSize;
                byte[] segment = new byte[count];
                Array.Copy(inputBytes, start, segment, 0, count);

                // Generate Key/IV
                using Aes aes = Aes.Create();
                aes.GenerateKey();
                aes.GenerateIV();
                byte[] key = aes.Key;
                byte[] iv = aes.IV;
                
                string keyFile = $"key_{tid}.bin";
                string ivFile = $"iv_{tid}.bin";
                File.WriteAllBytes(keyFile, key);
                File.WriteAllBytes(ivFile, iv);

                keyFiles[tid] = keyFile;
                ivFiles[tid] = ivFile;

                tasks[i] = Task.Run(() => EncryptSegment(segment, key, iv));
            }

            Task.WaitAll(tasks);

            List<byte> finalEncrypted = new List<byte>();
            StringBuilder keysInfo = new StringBuilder();
            keysInfo.AppendLine(numThreads.ToString());

            int currentEncryptedOffset = 0;
            for (int i = 0; i < numThreads; i++)
            {
                byte[] enc = tasks[i].Result;
                finalEncrypted.AddRange(enc);
                int size = enc.Length;
                
                keysInfo.AppendLine($"{currentEncryptedOffset} {currentEncryptedOffset + size} {keyFiles[i]} {ivFiles[i]}");
                currentEncryptedOffset += size;
            }

            File.WriteAllBytes(outputEncryptedFile, finalEncrypted.ToArray());
            File.WriteAllText(outputKeysFile, keysInfo.ToString());

            sw.Stop();
            Console.WriteLine($"Encryption with {numThreads} threads took {sw.ElapsedMilliseconds} ms");
        }

        static byte[] EncryptSegment(byte[] input, byte[] key, byte[] iv)
        {
            using Aes aes = Aes.Create();
            aes.Key = key;
            aes.IV = iv;
            using MemoryStream output = new();
            using CryptoStream cryptoStream = new(output, aes.CreateEncryptor(), CryptoStreamMode.Write);
            cryptoStream.Write(input);
            cryptoStream.FlushFinalBlock();
            return output.ToArray();
        }
    }
}