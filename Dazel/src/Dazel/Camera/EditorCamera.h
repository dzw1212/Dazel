#pragma once

#include "Camera.h"
#include "Dazel/Core/Timestep.h"
#include "Dazel/Events/Event.h"
#include "Dazel/Events/MouseEvent.h"

#include "glm/glm.hpp"

namespace DAZEL
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(const EditorCamera& camera) = default;
		EditorCamera(float fFov, float fAspectRatio, float fNearClip, float fFarClip);


		void OnUpdate(Timestep timeStep);
		void OnEvent(Event& event);

		inline float GetDistance() const { return m_fDistance; }
		inline void SetDistance(float fDistance) { m_fDistance = fDistance; }

		void SetViewportSize(float fWidth, float fHeight);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjMatrix() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetPosition() { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_fPitch; }
		float GetYaw() const { return m_fYaw; }
		float GetRoll() const { return m_fRoll; }


		void UpdateView();
		void UpdateProjection();
	
	private:
		bool OmMouseScroll(MouseScrolledEvent& event);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float fDelta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotateSpeed() const;
		float ZoomSpeed() const;

	private:
		const glm::vec3 x_axis = { 1.f, 0.f, 0.f };
		const glm::vec3 y_axis = { 0.f, 1.f, 0.f };
		const glm::vec3 z_axis = { 0.f, 0.f, 1.f };

	private:
		float m_fVerticalFOV = 45.f;
		float m_fAspectRatio = 1.f;
		float m_fNearClip = 0.1f;
		float m_fFarClip = 1000.f;

		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position = { 0.f, 0.f, -10.f };
		glm::vec3 m_FocalPoint = { 0.f, 0.f, 0.f };

		float m_fDistance = 10.f;

		float m_fPitch = 0.f;
		float m_fYaw = 0.f;
		float m_fRoll = 0.f;

		float m_fViewportWidth = 1.f;
		float m_fViewportHeight = 1.f;

		glm::vec2 m_InititalMousePosition;
	};
}
