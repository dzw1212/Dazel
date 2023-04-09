#include "DazelPCH.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"
#include "UniformBuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

namespace DAZEL
{
	Renderer2DData Renderer2D::s_Renderer2DData;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();


		//Quad
		s_Renderer2DData.QuadVertexArray = VertexArray::Create();

		BufferLayout squareLayout = {
			{ShaderDataType::VEC3,		"vertPos"},
			{ShaderDataType::VEC2,		"texCoord"},
			{ShaderDataType::VEC4,		"color"},
			{ShaderDataType::FLOAT,		"texIndex"},
			{ShaderDataType::FLOAT,		"tilingFactor"},
			{ShaderDataType::INT,		"entityId"},
		};

		s_Renderer2DData.QuadVertexBuffer = VertexBuffer::Create(s_Renderer2DData.uiMaxVertices * sizeof(QuadVertex));
		s_Renderer2DData.QuadVertexBuffer->SetLayout(squareLayout);
		s_Renderer2DData.QuadVertexArray->AddVertexBuffer(s_Renderer2DData.QuadVertexBuffer);
		s_Renderer2DData.pQuadVertexBufferBase = new QuadVertex[s_Renderer2DData.uiMaxVertices];


		//Circle
		s_Renderer2DData.CircleVertexArray = VertexArray::Create();

		BufferLayout circleLayout = {
			{ShaderDataType::VEC3,		"worldPos"},
			{ShaderDataType::VEC3,		"localPos"},
			{ShaderDataType::VEC4,		"color"},
			{ShaderDataType::FLOAT,		"thickness"},
			{ShaderDataType::FLOAT,		"fade"},
			{ShaderDataType::INT,		"entityId"},
		};

		s_Renderer2DData.CircleVertexBuffer = VertexBuffer::Create(s_Renderer2DData.uiMaxVertices * sizeof(CircleVertex));
		s_Renderer2DData.CircleVertexBuffer->SetLayout(circleLayout);
		s_Renderer2DData.CircleVertexArray->AddVertexBuffer(s_Renderer2DData.CircleVertexBuffer);
		s_Renderer2DData.pCircleVertexBufferBase = new CircleVertex[s_Renderer2DData.uiMaxVertices];

		//Line
		s_Renderer2DData.LineVertexArray = VertexArray::Create();

		BufferLayout lineLayout = {
			{ShaderDataType::VEC3,		"vertPos"},
			{ShaderDataType::VEC4,		"color"},
			{ShaderDataType::INT,		"entityId"},
		};

		s_Renderer2DData.LineVertexBuffer = VertexBuffer::Create(s_Renderer2DData.uiMaxVertices * sizeof(LineVertex));
		s_Renderer2DData.LineVertexBuffer->SetLayout(lineLayout);
		s_Renderer2DData.LineVertexArray->AddVertexBuffer(s_Renderer2DData.LineVertexBuffer);
		s_Renderer2DData.pLineVertexBufferBase = new LineVertex[s_Renderer2DData.uiMaxVertices];

		UINT* QuadIndices = new UINT[s_Renderer2DData.uiMaxIndices];
		UINT uiOffset = 0;
		for (UINT i = 0; i < s_Renderer2DData.uiMaxIndices; i += 6)
		{
			QuadIndices[i + 0] = uiOffset + 0;
			QuadIndices[i + 1] = uiOffset + 1;
			QuadIndices[i + 2] = uiOffset + 2;

			QuadIndices[i + 3] = uiOffset + 2;
			QuadIndices[i + 4] = uiOffset + 3;
			QuadIndices[i + 5] = uiOffset + 0;
			
			uiOffset += 4;
		}

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(QuadIndices, s_Renderer2DData.uiMaxIndices);
		s_Renderer2DData.QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
		s_Renderer2DData.CircleVertexArray->SetIndexBuffer(squareIndexBuffer);

		delete[] QuadIndices;

		s_Renderer2DData.QuadShader = Shader::Create("assert/shader/Quad.glsl");
		s_Renderer2DData.CircleShader = Shader::Create("assert/shader/Circle.glsl");
		s_Renderer2DData.LineShader = Shader::Create("assert/shader/Line.glsl");

		s_Renderer2DData.WhiteTexture = Texture2D::Create(1, 1);
		UINT whiteColor = 0xffffffff;
		s_Renderer2DData.WhiteTexture->SetData(&whiteColor, sizeof(whiteColor));

		int samplers[s_Renderer2DData.uiMaxTextureSlots];
		for (int i = 0; i < s_Renderer2DData.uiMaxTextureSlots; ++i)
		{
			samplers[i] = i;
		}
		//s_Renderer2DData.TextureShader->Bind();
		//s_Renderer2DData.TextureShader->SetIntArray("u_TextureArray", samplers, s_Renderer2DData.uiMaxTextureSlots);

		s_Renderer2DData.TextureSlots[0] = s_Renderer2DData.WhiteTexture;

		s_Renderer2DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(s_Renderer2DData.CameraBufferData), 0);
	}
	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();

	}
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		PROFILE_FUNCTION();

		s_Renderer2DData.CameraBufferData.ViewProjMat = camera.GetProjection() * glm::inverse(cameraTransform);
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraBufferData, sizeof(s_Renderer2DData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		PROFILE_FUNCTION();

		s_Renderer2DData.CameraBufferData.ViewProjMat = camera.GetViewProjMatrix();
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraBufferData, sizeof(s_Renderer2DData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		s_Renderer2DData.CameraBufferData.ViewProjMat = camera.GetViewProjMatrix();
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraBufferData, sizeof(s_Renderer2DData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const glm::mat4& viewProjMat)
	{
		PROFILE_FUNCTION();

		s_Renderer2DData.CameraBufferData.ViewProjMat = viewProjMat;
		s_Renderer2DData.CameraUniformBuffer->SetData(&s_Renderer2DData.CameraBufferData, sizeof(s_Renderer2DData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::EndScene()
	{
		PROFILE_FUNCTION();
		
		Flush();
	}
	void Renderer2D::Flush()
	{
		PROFILE_FUNCTION();

		

		if (s_Renderer2DData.uiQuadIndexCount > 0)
		{
			UINT uiDataSize = (s_Renderer2DData.pQuadVertexBufferPointer - s_Renderer2DData.pQuadVertexBufferBase) * (UINT)sizeof(QuadVertex);
			s_Renderer2DData.QuadVertexBuffer->SetData(s_Renderer2DData.pQuadVertexBufferBase, uiDataSize);

			for (UINT i = 0; i < s_Renderer2DData.uiTextureSlotIndex; ++i)
			{
				s_Renderer2DData.TextureSlots[i]->Bind(i);
			}

			s_Renderer2DData.QuadShader->Bind();
			RendererCommand::DrawIndexed(s_Renderer2DData.QuadVertexArray, s_Renderer2DData.uiQuadIndexCount);
			s_Renderer2DData.Stat.uiDrawCalls++;
		}

		if (s_Renderer2DData.uiCircleIndexCount > 0)
		{
			UINT uiDataSize = (s_Renderer2DData.pCircleVertexBufferPointer - s_Renderer2DData.pCircleVertexBufferBase) * (UINT)sizeof(CircleVertex);
			s_Renderer2DData.CircleVertexBuffer->SetData(s_Renderer2DData.pCircleVertexBufferBase, uiDataSize);

			s_Renderer2DData.CircleVertexArray->Bind();
			s_Renderer2DData.CircleShader->Bind();
			RendererCommand::DrawIndexed(s_Renderer2DData.CircleVertexArray, s_Renderer2DData.uiCircleIndexCount);
			s_Renderer2DData.Stat.uiDrawCalls++;
		}

		if (s_Renderer2DData.uiLineVertexCount > 0)
		{
			UINT uiDataSize = (s_Renderer2DData.pLineVertexBufferPointer - s_Renderer2DData.pLineVertexBufferBase) * (UINT)sizeof(LineVertex);
			s_Renderer2DData.LineVertexBuffer->SetData(s_Renderer2DData.pLineVertexBufferBase, uiDataSize);

			s_Renderer2DData.LineShader->Bind();
			RendererCommand::SetLineWidth(s_Renderer2DData.fLineWidth);
			RendererCommand::DrawLines(s_Renderer2DData.LineVertexArray, s_Renderer2DData.uiLineVertexCount);
			s_Renderer2DData.Stat.uiDrawCalls++;
		}
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int nEntityId)
	{
		PROFILE_FUNCTION();

		if (s_Renderer2DData.uiQuadIndexCount >= s_Renderer2DData.uiMaxIndices)
		{
			NextBatch();
		}

		constexpr float fWhiteTextureSlot = 0.f;
		constexpr float fPureColorTilingFactor = 1.f;

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pQuadVertexBufferPointer->Pos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->TexCoord = s_Renderer2DData.SquareVertexTexCoordData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->Color = color;
			s_Renderer2DData.pQuadVertexBufferPointer->fTexIndex = fWhiteTextureSlot;
			s_Renderer2DData.pQuadVertexBufferPointer->fTilingFactor = fPureColorTilingFactor;
			s_Renderer2DData.pQuadVertexBufferPointer->nEntityId = nEntityId;
			s_Renderer2DData.pQuadVertexBufferPointer++;
		}

		s_Renderer2DData.uiQuadIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor, int nEntityId)
	{
		if (s_Renderer2DData.uiQuadIndexCount >= s_Renderer2DData.uiMaxIndices)
		{
			NextBatch();
		}

		int nFindSlot = 0;
		for (UINT i = 1; i < s_Renderer2DData.uiTextureSlotIndex; ++i)
		{
			if (s_Renderer2DData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_Renderer2DData.uiTextureSlotIndex;
			s_Renderer2DData.TextureSlots[s_Renderer2DData.uiTextureSlotIndex] = texture;
			s_Renderer2DData.uiTextureSlotIndex++;
		}

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pQuadVertexBufferPointer->Pos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->TexCoord = s_Renderer2DData.SquareVertexTexCoordData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->Color = whiteColor;
			s_Renderer2DData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_Renderer2DData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_Renderer2DData.pQuadVertexBufferPointer->nEntityId = nEntityId;
			s_Renderer2DData.pQuadVertexBufferPointer++;
		}

		s_Renderer2DData.uiQuadIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(pos2, 0.f), size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));
		auto transform = translateMat * scaleMat;

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos2, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		DrawQuad(glm::vec3(pos2, 0.f), size, texture, nTileCoef, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		PROFILE_FUNCTION();

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));
		auto transform = translateMat * scaleMat;

		DrawQuad(transform, texture, nTileCoef, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& pos3, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, int nTileCoef, const glm::vec3& tintColor)
	{
		PROFILE_FUNCTION();

		if (s_Renderer2DData.uiQuadIndexCount >= s_Renderer2DData.uiMaxIndices)
		{
			NextBatch();
		}

		auto texture = subTexture->GetTexture();
		auto texCoords = subTexture->GetTexCoords();

		int nFindSlot = 0;
		for (UINT i = 1; i < s_Renderer2DData.uiTextureSlotIndex; ++i)
		{
			if (s_Renderer2DData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_Renderer2DData.uiTextureSlotIndex;
			s_Renderer2DData.TextureSlots[s_Renderer2DData.uiTextureSlotIndex] = texture;
			s_Renderer2DData.uiTextureSlotIndex++;
		}

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));
		auto transform = translateMat * scaleMat;

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pQuadVertexBufferPointer->Pos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->TexCoord = texCoords[i];
			s_Renderer2DData.pQuadVertexBufferPointer->Color = whiteColor;
			s_Renderer2DData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_Renderer2DData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_Renderer2DData.pQuadVertexBufferPointer++;
		}

		s_Renderer2DData.uiQuadIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float fThickness, float fFade, int nEntityId)
	{
		//if (s_Renderer2DData.uiCircleIndexCount >= s_Renderer2DData.uiMaxIndices)
		//{
		//	NextBatch();
		//}

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pCircleVertexBufferPointer->WorldPos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pCircleVertexBufferPointer->LocalPos = s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pCircleVertexBufferPointer->Color = color;
			s_Renderer2DData.pCircleVertexBufferPointer->fThickness = fThickness;
			s_Renderer2DData.pCircleVertexBufferPointer->fFade = fFade;
			s_Renderer2DData.pCircleVertexBufferPointer->nEntityId = nEntityId;
			s_Renderer2DData.pCircleVertexBufferPointer++;
		}

		s_Renderer2DData.uiCircleIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	float Renderer2D::GetLineWidth()
	{
		return s_Renderer2DData.fLineWidth;
	}
	void Renderer2D::SetLineWidth(float fWidth)
	{
		s_Renderer2DData.fLineWidth = fWidth;
	}
	void Renderer2D::DrawLine(const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec4& color, int nEntityId)
	{
		s_Renderer2DData.pLineVertexBufferPointer->Pos = startPoint;
		s_Renderer2DData.pLineVertexBufferPointer->Color = color;
		s_Renderer2DData.pLineVertexBufferPointer->nEntityId = nEntityId;
		s_Renderer2DData.pLineVertexBufferPointer++;

		s_Renderer2DData.pLineVertexBufferPointer->Pos = endPoint;
		s_Renderer2DData.pLineVertexBufferPointer->Color = color;
		s_Renderer2DData.pLineVertexBufferPointer->nEntityId = nEntityId;
		s_Renderer2DData.pLineVertexBufferPointer++;

		s_Renderer2DData.uiLineVertexCount += 2;
	}
	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int nEntityId)
	{
		glm::vec3 point[4];
		for (int i = 0; i < 4; ++i)
		{
			point[i] = transform * s_Renderer2DData.SquareVertexPosData[i];
		}
		DrawLine(point[0], point[1], color, nEntityId);
		DrawLine(point[1], point[2], color, nEntityId);
		DrawLine(point[2], point[3], color, nEntityId);
		DrawLine(point[3], point[0], color, nEntityId);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const glm::vec4& color)
	{
		DrawRotateQuad(glm::vec3(pos2, 0.f), size, fRotationRad, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto rotateMat = glm::rotate(glm::mat4(1.f), fRotationRad, glm::vec3(0.f, 0.f, 1.f));
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));

		auto transform = translateMat * rotateMat * scaleMat;

		if (s_Renderer2DData.uiQuadIndexCount >= s_Renderer2DData.uiMaxIndices)
		{
			NextBatch();
		}

		constexpr float fWhiteTextureSlot = 0.f;
		constexpr float fPureColorTilingFactor = 1.f;

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pQuadVertexBufferPointer->Pos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->TexCoord = s_Renderer2DData.SquareVertexTexCoordData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->Color = color;
			s_Renderer2DData.pQuadVertexBufferPointer->fTexIndex = fWhiteTextureSlot;
			s_Renderer2DData.pQuadVertexBufferPointer->fTilingFactor = fPureColorTilingFactor;
			s_Renderer2DData.pQuadVertexBufferPointer++;
		}

		s_Renderer2DData.uiQuadIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		DrawRotateQuad(glm::vec3(pos2, 0.f), size, fRotationRad, texture, nTileCoef, tintColor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		PROFILE_FUNCTION();

		if (s_Renderer2DData.uiQuadIndexCount >= s_Renderer2DData.uiMaxIndices)
		{
			NextBatch();
		}

		int nFindSlot = 0;
		for (UINT i = 1; i < s_Renderer2DData.uiTextureSlotIndex; ++i)
		{
			if (s_Renderer2DData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_Renderer2DData.uiTextureSlotIndex;
			s_Renderer2DData.TextureSlots[s_Renderer2DData.uiTextureSlotIndex] = texture;
			s_Renderer2DData.uiTextureSlotIndex++;
		}

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto rotateMat = glm::rotate(glm::mat4(1.f), fRotationRad, glm::vec3(0.f, 0.f, 1.f));
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));

		auto transform = translateMat * rotateMat * scaleMat;

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		for (int i = 0; i < 4; ++i)
		{
			s_Renderer2DData.pQuadVertexBufferPointer->Pos = transform * s_Renderer2DData.SquareVertexPosData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->TexCoord = s_Renderer2DData.SquareVertexTexCoordData[i];
			s_Renderer2DData.pQuadVertexBufferPointer->Color = whiteColor;
			s_Renderer2DData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_Renderer2DData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_Renderer2DData.pQuadVertexBufferPointer++;
		}

		s_Renderer2DData.uiQuadIndexCount += 6;

		s_Renderer2DData.Stat.uiQuadCount++;
	}
	void Renderer2D::StartBatch()
	{
		s_Renderer2DData.uiQuadIndexCount = 0;
		s_Renderer2DData.pQuadVertexBufferPointer = s_Renderer2DData.pQuadVertexBufferBase;

		s_Renderer2DData.uiCircleIndexCount = 0;
		s_Renderer2DData.pCircleVertexBufferPointer = s_Renderer2DData.pCircleVertexBufferBase;

		s_Renderer2DData.uiLineVertexCount = 0;
		s_Renderer2DData.pLineVertexBufferPointer = s_Renderer2DData.pLineVertexBufferBase;

		s_Renderer2DData.uiTextureSlotIndex = 1; //0已被白色贴图占据
	}
	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
}
