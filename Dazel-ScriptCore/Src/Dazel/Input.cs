namespace DAZEL
{
	public static class Input
	{
		public static bool IsKeyDown(KeyCode keycode)
		{
			return InternalCall.Input_IsKeyDown(keycode);
		}
	}
}
