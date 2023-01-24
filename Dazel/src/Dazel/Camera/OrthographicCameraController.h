#pragma once

#include "OrthographicCamera.h"

#include "Dazel/Events/ApplicationEvent.h"
#include "Dazel/Events/MouseEvent.h"

#include "Dazel/Core/Timestep.h"

namespace DAZEL
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float fAspectRatio, bool bRotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
	private:
		float m_fAspectRatio = 1.f;
		float m_fZoomLevel = 1.f;
		OrthographicCamera m_Camera;

		bool m_bRotation;

		float m_fCameraTranslateSpeed = 10.f;
		float m_fCameraRotateSpeed = 50.f;
	};
}