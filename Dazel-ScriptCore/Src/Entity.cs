using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DAZEL
{
	public class Entity
	{
		protected Entity()
		{
			m_Id = 0;
		}
		internal Entity(ulong id)
		{
			m_Id = id;
		}

		public readonly ulong m_Id;

		//public Vector3 Position
		//{
		//	get
		//	{
		//		InternalCall.TransformComponent_GetPosition(m_Id, out Vector3 pos);
		//	}
		//}
	}

	public class TestEntity : Entity
	{ 
		public void OnCreate()
		{
			Console.Write("TextEntity OnCreate in csharp\n");
			Console.Write($"TextEntity UUID =  {m_Id}\n");
		}

		public void OnUpdate(float fTimestep)
		{
			Console.Write("TextEntity OnUpdate in csharp, delta = {0}\n", fTimestep);
		}
	}

	public class spriteRendererEntity : Entity
	{ }
}
