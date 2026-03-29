using System.IO;

// Output path is relative to the published executable location: bin/Release/net8.0/
// 4 levels up => YourBindingGen/ folder.
// See https://github.com/EvergineTeam/OpenGL.NET for a full example of a simple binding generator.
string outputPath = Path.Combine("..", "..", "..", "..", "Evergine.Bindings.YourBinding", "Generated");

Directory.CreateDirectory(outputPath);

// TODO: Implement the generator logic.
//   1. Fetch or read any required spec (file, API documentation, etc.).
//   2. Parse the spec and write one or more .cs files into outputPath (e.g. Commands.cs, Enums.cs, Structs.cs).

Console.WriteLine($"Output path: {Path.GetFullPath(outputPath)}");
Console.WriteLine("Done.");
