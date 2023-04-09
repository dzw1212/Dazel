#pragma once

#include <memory>
#include "VertexArray.h"
#include "RendererAPI.h"

namespace DAZEL
{
	class RendererCommand
	{
	public:
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void SetViewport(int nX, int nY, int nWidth, int nHeight)
		{
			s_RendererAPI->SetViewport(nX, nY, nWidth, nHeight);
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, UINT uiIndexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, uiIndexCount);
		}
		inline static void SetLineWidth(float fWidth)
		{
			s_RendererAPI->SetLineWidth(fWidth);
		}
		inline static void DrawLines(const Ref<VertexArray>& vertexArray, UINT uiVertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, uiVertexCount);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}
