#include "DazelPCH.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace DAZEL
{
	void OpenGLRendererAPI::Init()
	{
		PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

	}
	void OpenGLRendererAPI::Clear()
	{
		PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::SetViewport(int nX, int nY, int nWidth, int nHeight)
	{
		PROFILE_FUNCTION();

		glViewport(nX, nY, nWidth, nHeight);
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, UINT uiIndexCount)
	{
		PROFILE_FUNCTION();
		if (uiIndexCount > 0)
			glDrawElements(GL_TRIANGLES, uiIndexCount, GL_UNSIGNED_INT, nullptr);
		else
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		
	}
}