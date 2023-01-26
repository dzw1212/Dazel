#pragma once

#include "glm/glm.hpp"
#include <memory>
#include "VertexArray.h"

namespace DAZEL
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			Vulkan,
			DirectX,
			Metal,
		};
	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void SetViewport(int nX, int nY, int nWidth, int nHeight) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, UINT uiIndexCount = 0) = 0;

		inline static API GetAPI() { return s_eAPI; }
	private:
		static API s_eAPI;
	};
}