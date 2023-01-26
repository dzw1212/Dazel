#pragma once

#include "Dazel/Renderer/RendererAPI.h"

namespace DAZEL
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void SetViewport(int nX, int nY, int nWidth, int nHeight) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, UINT uiIndexCount = 0) override;
	};
}