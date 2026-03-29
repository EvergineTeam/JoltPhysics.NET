using CppAst;
using System;
using System.Diagnostics;
using System.IO;

namespace JoltPhysicsGen
{
	class Program
	{
		static void Main(string[] args)
		{
			var headersDir = Path.Combine(AppContext.BaseDirectory, "Headers");
			var headerFile = Path.Combine(headersDir, "JoltC", "joltc.h");

			var options = new CppParserOptions
			{
				ParseMacros = true,
				IncludeFolders = { headersDir },
			};

			Console.WriteLine($"Parsing header: {headerFile}");
			var compilation = CppParser.ParseFile(headerFile, options);

			if (compilation.HasErrors)
			{
				foreach (var message in compilation.Diagnostics.Messages)
				{
					if (message.Type == CppLogMessageType.Error)
					{
						Console.ForegroundColor = ConsoleColor.Red;
					}

					Console.WriteLine(message);
					Console.ResetColor();
				}
			}

			Console.WriteLine($"Enums:     {compilation.Enums.Count}");
			Console.WriteLine($"Classes:   {compilation.Classes.Count}");
			Console.WriteLine($"Functions: {compilation.Functions.Count}");
			Console.WriteLine($"Typedefs:  {compilation.Typedefs.Count}");
			Console.WriteLine($"Macros:    {compilation.Macros.Count}");

			string outputPath = Path.Combine(
				AppContext.BaseDirectory,
				"..", "..", "..", "..", "..",
				"Evergine.Bindings.JoltPhysics", "Generated");

			if (!Directory.Exists(outputPath))
				Directory.CreateDirectory(outputPath);

			CsCodeGenerator.Instance.Generate(compilation, outputPath);

			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine($"\nGenerated files written to: {Path.GetFullPath(outputPath)}");
			Console.ResetColor();
		}
	}
}
