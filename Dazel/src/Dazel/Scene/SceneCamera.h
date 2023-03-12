#pragma once

#include "Dazel/Camera/Camera.h"

namespace DAZEL
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		SceneCamera(const glm::mat4& proj);
		virtual ~SceneCamera() = default;

		void SetOrthographic(float fSize, float fNear, float fFar);
		void SetPerspective(float fFovDeg, float fNear, float fFar);

		void SetViewportSize(UINT uiWidth, UINT uiHeight);
		void RecalculateProjection();

		float GetAspectRatio() const { return m_fAspectRatio; }
		void SetAspectRatio(float fAspectRatio);

		float GetOrthographicSize() const { return m_fOrthographicSize; }
		void SetOrthographicSize(float fSize);

		float GetOrthographicNearClip() const { return m_fOrthographicNear; }
		void SetOrthographicNearClip(float fNear);
		
		float GetOrthographicFarClip() const { return m_fOrthographicFar; }
		void SetOrthographicFarClip(float fFar);

		float GetPerspectiveVerticalFOV() const { return glm::degrees(m_fPerspectiveVerticalFOV); }
		void SetPerspectiveVerticalFOV(float fDegree);

		float GetPerspectiveNearClip() const { return m_fPerspectiveNear; }
		void SetPerspectiveNearClip(float fNear);

		float GetPerspectiveFarClip() const { return m_fPerspectiveFar; }
		void SetPerspectiveFarClip(float fFar);

	public:
		enum class ProjectionType : int
		{
			Orthographic = 0,
			Perspective,
			Max,
		};

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type);
	private:
		float m_fOrthographicSize = 10.f;
		float m_fOrthographicNear = -1.f;
		float m_fOrthographicFar = 1.f;

		float m_fAspectRatio = 1.f;

		float m_fPerspectiveNear = 0.01f;
		float m_fPerspectiveFar = 1000.f;
		float m_fPerspectiveVerticalFOV = glm::radians(45.f);

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
	};

}