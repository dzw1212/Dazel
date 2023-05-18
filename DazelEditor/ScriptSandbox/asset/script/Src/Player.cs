using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DAZEL;

namespace Sandbox
{
	public class Player : Entity
	{
		private TransformComponent m_Transform;
		private RigidBody2DComponent m_Rigidbody;
		public void OnCreate()
		{
			Console.Write("OnCreate\n");
			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<RigidBody2DComponent>();
		}

		public void OnUpdate(float fTimestep)
		{
			float speed = 1.0f;
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

			if (velocity.X == 0 && velocity.Y == 0)
				return;

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);

		}
	}
}
