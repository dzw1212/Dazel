#pragma once

#include "Dazel/Camera/OrthographicCamera.h"
#include "Dazel/Camera/Camera.h"
#include "Dazel/Camera/EditorCamera.h"
#include "glm/glm.hpp"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture2D.h"

#include "UniformBuffer.h"

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

	struct CircleVertex
	{
		glm::vec3 WorldPos;
		glm::vec3 LocalPos;
		glm::vec4 Color;
		float fThickness;
		float fFade;

		int nEntityId;
	};

	struct LineVertex
	{
		glm::vec3 Pos;
		glm::vec4 Color;

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
		Ref<Texture> WhiteTexture;

		//Quad
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		UINT uiQuadIndexCount = 0;
		QuadVertex* pQuadVertexBufferBase = nullptr;
		QuadVertex* pQuadVertexBufferPointer = nullptr;

		//Circle
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		UINT uiCircleIndexCount = 0;
		CircleVertex* pCircleVertexBufferBase = nullptr;
		CircleVertex* pCircleVertexBufferPointer = nullptr;

		//Line
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		float fLineWidth = 2.f;

		UINT uiLineVertexCount = 0;
		LineVertex* pLineVertexBufferBase = nullptr;
		LineVertex* pLineVertexBufferPointer = nullptr;

		//Limit
		const UINT uiMaxQuads = 10000;
		const UINT uiMaxVertices = uiMaxQuads * 4;
		const UINT uiMaxIndices = uiMaxQuads * 6;

		static const UINT uiMaxTextureSlots = 32;
		std::array<Ref<Texture>, uiMaxTextureSlots> TextureSlots;
		UINT uiTextureSlotIndex = 1;

		Statistics Stat;

		std::array<glm::vec4, 4> SquareVertexPosData = {
			glm::vec4(-0.5f, -0.5f, 0.f, 1.f),
			glm::vec4(0.5f, -0.5f, 0.f, 1.f),
			glm::vec4(0.5f, 0.5f, 0.f, 1.f),
			glm::vec4(-0.5f, 0.5f, 0.f, 1.f),
		};

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
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f), int nEntityId = -1);

		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		
		static void DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float fThickness, float fFade, int nEntityId = -1);

		static float GetLineWidth();
		static void SetLineWidth(float fWidth);
		static void DrawLine(const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec4& color, int nEntityId = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int nEntityId = -1);

		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));
		static void DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef = 1, const glm::vec3& tintColor = glm::vec3(1.f));

	public:
		static void ResetStatisticData() { s_Renderer2DData.Stat.Reset(); }
		static Statistics GetStatisticData() { return s_Renderer2DData.Stat; }

	private:
		static void StartBatch();
		static void NextBatch();

	public:
		static Renderer2DData s_Renderer2DData;
	};
}
