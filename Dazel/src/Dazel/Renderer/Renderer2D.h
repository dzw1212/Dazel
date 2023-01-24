#pragma once

#include "Dazel/Camera/OrthographicCamera.h"
#include "glm/glm.hpp"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace DAZEL
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));

		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
	public:
		struct Renderer2DData
		{
			Ref<VertexArray> SquareVertexArray;
			Ref<Shader> TextureShader;
			Ref<Texture> WhiteTexture;
		};
	};
}