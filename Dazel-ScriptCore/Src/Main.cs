using System;
using System.Runtime.CompilerServices;

namespace DAZEL
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main default constructor");
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello World From C#");

			CppFunction();
		}

		public void PrintCustomMessage(string msg)
		{
			Console.WriteLine($"C# says: {msg}");
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static void CppFunction();
	}

}

