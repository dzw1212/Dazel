﻿using System;

public class Main
{
	public float FloatVar { get; set; }

	public Main()
	{
		Console.WriteLine("Main constructor");
	}

	public void PrintMessage()
	{
		Console.WriteLine("Hello World From C#");
	}

	public void PrintCustomMessage(string msg)
	{
		Console.WriteLine($"C# says: {msg}");
	}
}
