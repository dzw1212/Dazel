#pragma once

#include "glm/glm.hpp"

namespace DAZEL
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float fLeft, float fRight, float fBottom, float fTop);

		void SetPosition(const glm::vec3& pos)
		{
			m_Position = pos;
			RecalculateViewMatrix();
		}
		glm::vec3 GetPosition() const { return m_Position; }

		void SetRotation(float fRotationDeg)
		{ 
			m_fRotationDeg = fRotationDeg;
			RecalculateViewMatrix();
		}
		float GetRotationDeg() const { return m_fRotationDeg; }

		void SetProjectionMatrix(float fLeft, float fRight, float fBottom, float fTop);

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMatrix;

		glm::vec3 m_Position;
		float m_fRotationDeg = 0.f;
	};
}