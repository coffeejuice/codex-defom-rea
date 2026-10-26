// Minimal C# caller:  m2m_sample_csharp <project.moproj> [out.MST]
using System;
using Moproj2Mst;

static class Sample
{
    static int Main(string[] args)
    {
        if (args.Length < 1)
        {
            Console.Error.WriteLine("usage: m2m_sample_csharp <project.moproj> [out.MST]");
            return (int)M2MResult.BadArgument;
        }
        Console.WriteLine("api " + Converter.ApiVersion);
        Console.WriteLine("unit " + (int)Converter.SetGuiUnit(0));   // 0 = as the original derives it
        string message;
        M2MResult rc = Converter.Convert(args[0], args.Length > 1 ? args[1] : null, M2MFlags.None, out message);
        Console.WriteLine("result " + (int)rc + " (" + rc + ")");
        if (message.Length > 0) Console.WriteLine(message);
        return (int)rc;
    }
}
