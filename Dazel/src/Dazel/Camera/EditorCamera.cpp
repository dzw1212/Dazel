#include "DazelPCH.h"

#include "EditorCamera.h"

#include "Dazel/Core/Input.h"
#include "Dazel/Core/KeyCodes.h"
#include "Dazel/Core/MouseButtonCodes.h"

#include "GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace DAZEL
{
	EditorCamera::EditorCamera(float fFov, float fAspectRatio, float fNearClip, float fFarClip)
		: m_fVerticalFOV(fFov), m_fAspectRatio(fAspectRatio), m_fNearClip(fNearClip), m_fFarClip(fFarClip)
	{
		UpdateProjection();
	}
	void EditorCamera::OnUpdate(Timestep timeStep)
	{
		if (Input::IsKeyPressed(DAZEL_KEY_LEFT_CONTROL))
		{
			const glm::vec2& curMousePos{ Input::GetMousePos().first, Input::GetMousePos().second };
			glm::vec2 delta = (curMousePos - m_InititalMousePosition) * 0.003f;
			m_InititalMousePosition = curMousePos;

			if (Input::IsMouseButtonPressed(DAZEL_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(DAZEL_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(DAZEL_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}

		UpdateView();
	}
	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::OmMouseScroll));
	}
	void EditorCamera::SetViewportSize(float fWidth, float fHeight)
	{
		m_fViewportWidth = fWidth;
		m_fViewportHeight = fHeight;
		UpdateProjection();
	}
	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), y_axis);
	}
	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), x_axis);
	}
	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), -1.f * z_axis);
	}
	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(-1.f * glm::vec3(m_fPitch, m_fYaw, m_fRoll));
	}
	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		auto transform = glm::translate(glm::mat4(1.f), m_Position) * glm::toMat4(GetOrientation());
		m_ViewMatrix = glm::inverse(transform);
	}
	void EditorCamera::UpdateProjection()
	{
		CORE_ASSERT(m_fViewportHeight != 0.f, "Viewport Height cant be 0!");
		m_fAspectRatio = m_fViewportWidth / m_fViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_fVerticalFOV), m_fAspectRatio, m_fNearClip, m_fFarClip);
	}
	bool EditorCamera::OmMouseScroll(MouseScrolledEvent& event)
	{
		float fDelta = event.GetOffsetY() * 0.1f;
		MouseZoom(fDelta);
		UpdateView();

		return false;
	}
	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_fDistance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_fDistance;
	}
	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float fYawSign = GetUpDirection().y < 0 ? -1.f : 1.f;
		m_fYaw += fYawSign * delta.x * RotateSpeed();
		m_fPitch += delta.y * RotateSpeed();
	}
	void EditorCamera::MouseZoom(float fDelta)
	{
		m_fDistance -= fDelta * ZoomSpeed();
		if (m_fDistance < 1.f)
		{
			m_FocalPoint += GetForwardDirection();
			m_fDistance = 1.f;
		}
	}
	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_fDistance;
	}
	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_fViewportWidth / 1000.f, 2.4f);
		float xFactor = 0.00366f * (x * x) - 0.1778f * x + 0.3021f;
		
		float y = std::min(m_fViewportHeight / 1000.f, 2.4f);
		float yFactor = 0.00366f * (y * y) - 0.1778f * y + 0.3021f;

		return std::make_pair(xFactor, yFactor);
	}
	float EditorCamera::RotateSpeed() const
	{
		return 0.8f;
	}
	float EditorCamera::ZoomSpeed() const
	{
		float fDistance = std::max(m_fDistance * 0.2f, 0.f);
		float fSpeed = std::min(fDistance * fDistance, 100.f);
		return fSpeed;
	}
}