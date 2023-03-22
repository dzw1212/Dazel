#pragma once

#include "Dazel/Camera/OrthographicCamera.h"
#include "Dazel/Camera/Camera.h"
#include "Dazel/Camera/EditorCamera.h"
#include "glm/glm.hpp"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace DAZEL
{
	struct CameraData
	{
		glm::mat4 ViewProjMat;
	};

	struct QuadVertex
	{
		glm::vec3 Pos;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float fTexIndex;
		float fTilingFactor;

		int nEntityId;
	};
	struct Statistics
	{
		void Reset()
		{
			uiDrawCalls = 0;
			uiQuadCount = 0;
		}
		UINT uiDrawCalls = 0;
		UINT uiQuadCount = 0;

		UINT GetVertexCount() { return uiQuadCount * 4; }
		UINT GetIndexCount() { return uiQuadCount * 6; }
	};
	struct Renderer2DData
	{
		Ref<VertexArray> SquareVertexArray;
		Ref<VertexBuffer> SquareVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture> WhiteTexture;

		const UINT uiMaxQuads = 10000;
		const UINT uiMaxVertices = uiMaxQuads * 4;
		const UINT uiMaxIndices = uiMaxQuads * 6;


		UINT uiQuadIndexCount = 0;
		QuadVertex* pQuadVertexBufferBase = nullptr;;
		QuadVertex* pQuadVertexBufferPointer = nullptr;

		static const UINT uiMaxTextureSlots = 32;
		std::array<Ref<Texture>, uiMaxTextureSlots> TextureSlots;
		UINT uiTextureSlotIndex = 1;

		Statistics Stat;

		//std::array<glm::vec4, 4> SquareVertexPosData = {
		//	glm::vec4(-0.5f, 0.5f, 0.f, 1.f),
		//	glm::vec4(0.5f, 0.5f, 0.f, 1.f),
		//	glm::vec4(0.5f, -0.5f, 0.f, 1.f),
		//	glm::vec4(-0.5f, -0.5f, 0.f, 1.f),
		//};
		std::array<glm::vec4, 4> SquareVertexPosData = {
			glm::vec4(-0.5f, -0.5f, 0.f, 1.f),
			glm::vec4(0.5f, -0.5f, 0.f, 1.f),
			glm::vec4(0.5f, 0.5f, 0.f, 1.f),
			glm::vec4(-0.5f, 0.5f, 0.f, 1.f),
		};
		//std::array<glm::vec2, 4> SquareVertexTexCoordData = {
		//	glm::vec2(0.f, 1.f),
		//	glm::vec2(1.f, 1.f),
		//	glm::vec2(1.f, 0.f),
		//	glm::vec2(0.f, 0.f),
		//};
		std::array<glm::vec2, 4> SquareVertexTexCoordData = {
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(0.f, 1.f),
		};

		CameraData CameraBufferData;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();


		static void BeginScene(const Camera& camera, const glm::mat4& cameraTransform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const glm::mat4& viewProjMat);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int nEntityId = -1);

		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));


		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));

	public:
		static void ResetStatisticData() { s_SquareData.Stat.Reset(); }
		static Statistics GetStatisticData() { return s_SquareData.Stat; }

	private:
		static void StartBatch();
		static void NextBatch();

	public:
		static Renderer2DData s_SquareData;
	};
}
