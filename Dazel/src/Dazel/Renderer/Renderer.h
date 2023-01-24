#pragma once

#include "RendererAPI.h"

#include <memory>
#include "VertexArray.h"
#include "Shader.h"

#include "Dazel/Camera/OrthographicCamera.h"

namespace DAZEL
{
	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void SetViewport(int nX, int nY, int nWidth, int nHeight);

		static void Submit(const Ref<VertexArray>& vertexArray, 
			const Ref<Shader>& shader, 
			const glm::mat4& modelMatrix);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjMatrix;
		};
		static SceneData* m_SceneData;
	};
}