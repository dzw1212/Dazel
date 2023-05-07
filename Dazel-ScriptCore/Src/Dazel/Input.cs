using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DAZEL
{
	static class Input
	{
		public static bool IsKeyDown(KeyCode keycode)
		{
			return InternalCall.Input_IsKeyDown(keycode);
		}
	}
}
