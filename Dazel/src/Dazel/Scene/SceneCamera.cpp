#include "DazelPCH.h"
#include "SceneCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace DAZEL
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}
	SceneCamera::SceneCamera(const glm::mat4& proj)
		: Camera(proj)
	{
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographic(float fSize, float fNear, float fFar)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_fOrthographicSize = fSize;
		m_fOrthographicNear = fNear;
		m_fOrthographicFar = fFar;

		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float fFovDeg, float fNear, float fFar)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_fPerspectiveVerticalFOV = glm::radians(fFovDeg);
		m_fPerspectiveNear = fNear;
		m_fPerspectiveFar = fFar;

		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(UINT uiWidth, UINT uiHeight)
	{
		m_fAspectRatio = (float)uiWidth / (float)uiHeight;

		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float fLeft = m_fOrthographicSize * m_fAspectRatio * -0.5f;
			float fRight = m_fOrthographicSize * m_fAspectRatio * 0.5f;
			float fBottom = m_fOrthographicSize * -0.5f;
			float fTop = m_fOrthographicSize * 0.5f;

			m_Projection = glm::ortho(fLeft, fRight, fBottom, fTop, m_fOrthographicNear, m_fOrthographicFar);
		}
		else if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_fPerspectiveVerticalFOV, m_fAspectRatio, m_fPerspectiveNear, m_fPerspectiveFar);
		}
	}
	void SceneCamera::SetAspectRatio(float fAspectRatio)
	{
		m_fAspectRatio = fAspectRatio;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographicSize(float fSize)
	{
		m_fOrthographicSize = fSize;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographicNearClip(float fNear)
	{
		m_fOrthographicNear = fNear;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographicFarClip(float fFar)
	{
		m_fOrthographicFar = fFar;
		RecalculateProjection();
	}
	void SceneCamera::SetPerspectiveVerticalFOV(float fDegree)
	{
		m_fPerspectiveVerticalFOV = glm::radians(fDegree);
		RecalculateProjection();
	}
	void SceneCamera::SetPerspectiveNearClip(float fNear)
	{
		m_fPerspectiveNear = fNear;
		RecalculateProjection();
	}
	void SceneCamera::SetPerspectiveFarClip(float fFar)
	{
		m_fPerspectiveFar = fFar;
		RecalculateProjection();
	}
	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		RecalculateProjection();
	}
}