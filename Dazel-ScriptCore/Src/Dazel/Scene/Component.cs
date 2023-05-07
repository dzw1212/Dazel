using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DAZEL
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

	public class TransformComponent : Component
	{
		public Vector3 Translation
		{
			get
			{
				InternalCall.TransformComponent_GetPosition(Entity.m_Id, out Vector3 translation);
				return translation;
			}
			set
			{
				InternalCall.TransformComponent_SetPosition(Entity.m_Id, ref value);
			}
		}
	}

	public class Rigidbody2DComponent : Component
	{

		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
		{
			InternalCall.Rigidbody2DComponent_ApplyLinearImpulse(Entity.m_Id, ref impulse, ref worldPosition, wake);
		}

		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCall.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.m_Id, ref impulse, wake);
		}

	}
}
