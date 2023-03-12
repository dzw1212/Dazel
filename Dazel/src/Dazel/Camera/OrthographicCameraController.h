#pragma once

#include "OrthographicCamera.h"

#include "Dazel/Events/ApplicationEvent.h"
#include "Dazel/Events/MouseEvent.h"

#include "Dazel/Core/Timestep.h"

namespace DAZEL
{
	struct OrthographicCameraBounds
	{
		OrthographicCameraBounds(float fLeft, float fRight, float fBottom, float fTop)
			: fLeft(fLeft), fRight(fRight), fBottom(fBottom), fTop(fTop)
		{

		}

		float GetWidth() { return fRight - fLeft; }
		float GetHeight() { return fTop - fBottom; }

		float fLeft, fRight, fTop, fBottom;
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float fAspectRatio, bool bRotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_fZoomLevel; }
		void SetZoomLevel(float fLevel);

		float GetAspectRatio() const { return m_fAspectRatio; }
		void SetAspectRatio(float fRatio);

		const OrthographicCameraBounds GetBounds() const { return m_Bounds; }

		void CalculateView();
	private:
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
	private:
		float m_fAspectRatio = 1.f;
		float m_fZoomLevel = 1.f;
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;

		bool m_bRotation;

		float m_fCameraTranslateSpeed = 10.f;
		float m_fCameraRotateSpeed = 50.f;
	};
}