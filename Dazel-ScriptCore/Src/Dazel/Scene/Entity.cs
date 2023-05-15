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

	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_Rigidbody;
		public void OnCreate()
		{
			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
		}

		public void OnUpdate(float fTimestep)
		{
			float speed = 0.01f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			velocity *= speed;

			Console.Write("x = {0}, y = {1}, z = {2}\n", velocity.X, velocity.Y, velocity.Z);

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);
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
