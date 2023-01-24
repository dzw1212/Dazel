#include "DazelPCH.h"
#include "OrthographicCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

namespace DAZEL
{
	OrthographicCamera::OrthographicCamera(float fLeft, float fRight, float fBottom, float fTop)
		: m_ProjectionMatrix(glm::ortho(fLeft, fRight, fBottom, fTop, -1.f, 1.f)), m_Position(0.f)
	{
		PROFILE_FUNCTION();

		RecalculateViewMatrix();
	}
	void OrthographicCamera::SetProjectionMatrix(float fLeft, float fRight, float fBottom, float fTop)
	{
		PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, -1.f, 1.f);
		m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		PROFILE_FUNCTION();

		//View = inverse(T * R)
		auto transform = glm::translate(glm::mat4(1.f), m_Position) 
			* glm::rotate(glm::mat4(1.f), glm::radians(m_fRotationDeg), glm::vec3(0.f, 0.f, 1.f)); //2DÖ»ÄÜÈÆzÖáÐý×ª
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}