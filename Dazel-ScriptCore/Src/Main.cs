using System;
using System.Runtime.CompilerServices;

namespace DAZEL
{
	public struct Vector3
	{
		public float X, Y, Z;

		public Vector3(float x , float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	}

	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CppFunction();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static void Log(ref Vector3 param);
	}

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
			InternalCalls.CppFunction();
		}

		public void PrintCustomMessage(string msg)
		{
			Console.WriteLine($"C# says: {msg}");
		}
	}

}

