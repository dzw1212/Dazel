#include "DazelPCH.h"
#include "OrthographicCameraController.h"

#include "Dazel/Core/Input.h"
#include "Dazel/Core/KeyCodes.h"

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

namespace DAZEL
{
	OrthographicCameraController::OrthographicCameraController(float fAspectRatio, bool bRotation)
		: m_fAspectRatio(fAspectRatio), m_fZoomLevel(1.0),
		  m_Camera(-m_fAspectRatio * m_fZoomLevel, m_fAspectRatio * m_fZoomLevel, -m_fZoomLevel, m_fZoomLevel), 
		  m_bRotation(bRotation)
	{
		PROFILE_FUNCTION();
	}
	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		PROFILE_FUNCTION();

		float tTimeStep = timestep.GetSeconds();
		float fMoveSpeed = m_fCameraTranslateSpeed * tTimeStep;
		
		auto cameraPos = m_Camera.GetPosition();

		if (Input::IsKeyPressed(DAZEL_KEY_W))
			cameraPos.y += fMoveSpeed;
		else if (Input::IsKeyPressed(DAZEL_KEY_A))
			cameraPos.x -= fMoveSpeed;
		else if (Input::IsKeyPressed(DAZEL_KEY_S))
			cameraPos.y -= fMoveSpeed;
		else if (Input::IsKeyPressed(DAZEL_KEY_D))
			cameraPos.x += fMoveSpeed;

		m_Camera.SetPosition(cameraPos);

		if (m_bRotation)
		{
			float fRotateSpeed = m_fCameraRotateSpeed * tTimeStep;
			auto cameraRotation = m_Camera.GetRotationDeg();
			if (Input::IsKeyPressed(DAZEL_KEY_Q))
				cameraRotation -= fRotateSpeed;
			else if (Input::IsKeyPressed(DAZEL_KEY_E))
				cameraRotation += fRotateSpeed;

			m_Camera.SetRotation(cameraRotation);
		}
	}
	void OrthographicCameraController::OnEvent(Event& event)
	{
		PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_fAspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjectionMatrix(-m_fAspectRatio * m_fZoomLevel, m_fAspectRatio * m_fZoomLevel, -m_fZoomLevel, m_fZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_fZoomLevel -= event.GetOffsetY();
		m_fZoomLevel = std::clamp(m_fZoomLevel, 0.25f, 10.f);
		m_Camera.SetProjectionMatrix(-m_fAspectRatio * m_fZoomLevel, m_fAspectRatio * m_fZoomLevel, -m_fZoomLevel, m_fZoomLevel);
		return false;
	}
}