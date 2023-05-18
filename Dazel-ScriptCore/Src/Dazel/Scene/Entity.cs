using System;
using System.Runtime.CompilerServices;

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

		public Vector3 Translation
		{
			get
			{
				InternalCall.TransformComponent_GetPosition(m_Id, out Vector3 result);
				return result;
			}
			set
			{
				InternalCall.TransformComponent_SetPosition(m_Id, ref value);
			}
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type componentType = typeof(T);
			return InternalCall.Entity_HasComponent(m_Id, componentType);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Entity = this };
			return component;
		}
	}
}
