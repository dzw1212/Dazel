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

	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main default constructor");

			Vector3 pos = new Vector3(1, 2, 3);
			Log(ref pos);
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello World From C#");

			
		}

		public void PrintCustomMessage(string msg)
		{
			Console.WriteLine($"C# says: {msg}");
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static void CppFunction();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static void Log(ref Vector3 param);
	}

}

