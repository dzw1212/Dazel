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

		public Vector3 Position
		{
			get
			{
				InternalCall.TransformComponent_GetPosition(m_Id, out Vector3 pos);
				return pos;
			}

			set
			{
				InternalCall.TransformComponent_SetPosition(m_Id, ref value);
			}
		}
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
			float speed = 0.01f;
			Vector3 pos = Position;
			if (Input.IsKeyDown(KeyCode.W))
				pos.Y -= speed;
			else if (Input.IsKeyDown(KeyCode.S))
				pos.Y += speed;
			Position = pos;
		}
	}

	public class spriteRendererEntity : Entity                                                                                         
	{ }
}
