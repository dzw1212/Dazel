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

		glEnable(GL_LINE_SMOOTH);

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
		vertexArray->Bind();
		UINT tmp = vertexArray->GetIndexBuffer()->GetCount();
		UINT uiCount = uiIndexCount > 0 ? uiIndexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, uiCount, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::SetLineWidth(float fWidth)
	{
		glLineWidth(fWidth);
	}
	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, UINT uiVertexCount)
	{
		PROFILE_FUNCTION();
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, uiVertexCount);
	}
}
