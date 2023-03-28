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
	Renderer2DData Renderer2D::s_SquareData;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();

		s_SquareData.SquareVertexArray = VertexArray::Create();

		BufferLayout squareLayout = {
			{ShaderDataType::VEC3,		"vertPos"},
			{ShaderDataType::VEC2,		"texCoord"},
			{ShaderDataType::VEC4,		"color"},
			{ShaderDataType::FLOAT,		"texIndex"},
			{ShaderDataType::FLOAT,		"tilingFactor"},
			{ShaderDataType::INT,		"entityId"},
		};

		s_SquareData.SquareVertexBuffer = VertexBuffer::Create(s_SquareData.uiMaxVertices * sizeof(QuadVertex));
		s_SquareData.SquareVertexBuffer->SetLayout(squareLayout);
		s_SquareData.SquareVertexArray->AddVertexBuffer(s_SquareData.SquareVertexBuffer);

		s_SquareData.pQuadVertexBufferBase = new QuadVertex[s_SquareData.uiMaxVertices];

		UINT* QuadIndices = new UINT[s_SquareData.uiMaxIndices];
		UINT uiOffset = 0;
		for (UINT i = 0; i < s_SquareData.uiMaxIndices; i += 6)
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
		squareIndexBuffer = IndexBuffer::Create(QuadIndices, s_SquareData.uiMaxIndices);
		s_SquareData.SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		delete[] QuadIndices;

		s_SquareData.TextureShader = Shader::Create("assert/shader/TextureShader_common.glsl");


		s_SquareData.WhiteTexture = Texture2D::Create(1, 1);
		UINT whiteColor = 0xffffffff;
		s_SquareData.WhiteTexture->SetData(&whiteColor, sizeof(whiteColor));

		int samplers[s_SquareData.uiMaxTextureSlots];
		for (int i = 0; i < s_SquareData.uiMaxTextureSlots; ++i)
		{
			samplers[i] = i;
		}
		//s_SquareData.TextureShader->Bind();
		//s_SquareData.TextureShader->SetIntArray("u_TextureArray", samplers, s_SquareData.uiMaxTextureSlots);

		s_SquareData.TextureSlots[0] = s_SquareData.WhiteTexture;

		s_SquareData.CameraUniformBuffer = UniformBuffer::Create(sizeof(s_SquareData.CameraBufferData), 0);
	}
	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();

	}
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		PROFILE_FUNCTION();

		s_SquareData.CameraBufferData.ViewProjMat = camera.GetProjection() * glm::inverse(cameraTransform);
		s_SquareData.CameraUniformBuffer->SetData(&s_SquareData.CameraBufferData, sizeof(s_SquareData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		PROFILE_FUNCTION();

		s_SquareData.CameraBufferData.ViewProjMat = camera.GetViewProjMatrix();
		s_SquareData.CameraUniformBuffer->SetData(&s_SquareData.CameraBufferData, sizeof(s_SquareData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		s_SquareData.CameraBufferData.ViewProjMat = camera.GetViewProjMatrix();
		s_SquareData.CameraUniformBuffer->SetData(&s_SquareData.CameraBufferData, sizeof(s_SquareData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::BeginScene(const glm::mat4& viewProjMat)
	{
		PROFILE_FUNCTION();

		s_SquareData.CameraBufferData.ViewProjMat = viewProjMat;
		s_SquareData.CameraUniformBuffer->SetData(&s_SquareData.CameraBufferData, sizeof(s_SquareData.CameraBufferData));

		StartBatch();
	}
	void Renderer2D::EndScene()
	{
		PROFILE_FUNCTION();

		UINT uiDataSize = (s_SquareData.pQuadVertexBufferPointer - s_SquareData.pQuadVertexBufferBase) * (UINT)sizeof(QuadVertex);
		s_SquareData.SquareVertexBuffer->SetData(s_SquareData.pQuadVertexBufferBase, uiDataSize);
		
		Flush();
	}
	void Renderer2D::Flush()
	{
		PROFILE_FUNCTION();

		s_SquareData.SquareVertexArray->Bind();

		for (UINT i = 0; i < s_SquareData.uiTextureSlotIndex; ++i)
		{
			s_SquareData.TextureSlots[i]->Bind(i);
		}

		if (s_SquareData.uiQuadIndexCount > 0)
		{
			s_SquareData.TextureShader->Bind();
			RendererCommand::DrawIndexed(s_SquareData.SquareVertexArray, s_SquareData.uiQuadIndexCount);
			s_SquareData.Stat.uiDrawCalls++;
		}
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int nEntityId)
	{
		PROFILE_FUNCTION();

		if (s_SquareData.uiQuadIndexCount >= s_SquareData.uiMaxIndices)
		{
			NextBatch();
		}

		constexpr float fWhiteTextureSlot = 0.f;
		constexpr float fPureColorTilingFactor = 1.f;

		for (int i = 0; i < 4; ++i)
		{
			s_SquareData.pQuadVertexBufferPointer->Pos = transform * s_SquareData.SquareVertexPosData[i];
			s_SquareData.pQuadVertexBufferPointer->TexCoord = s_SquareData.SquareVertexTexCoordData[i];
			s_SquareData.pQuadVertexBufferPointer->Color = color;
			s_SquareData.pQuadVertexBufferPointer->fTexIndex = fWhiteTextureSlot;
			s_SquareData.pQuadVertexBufferPointer->fTilingFactor = fPureColorTilingFactor;
			s_SquareData.pQuadVertexBufferPointer->nEntityId = nEntityId;
			s_SquareData.pQuadVertexBufferPointer++;
		}

		s_SquareData.uiQuadIndexCount += 6;

		s_SquareData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor, int nEntityId)
	{
		if (s_SquareData.uiQuadIndexCount >= s_SquareData.uiMaxIndices)
		{
			NextBatch();
		}

		int nFindSlot = 0;
		for (UINT i = 1; i < s_SquareData.uiTextureSlotIndex; ++i)
		{
			if (s_SquareData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_SquareData.uiTextureSlotIndex;
			s_SquareData.TextureSlots[s_SquareData.uiTextureSlotIndex] = texture;
			s_SquareData.uiTextureSlotIndex++;
		}

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		for (int i = 0; i < 4; ++i)
		{
			s_SquareData.pQuadVertexBufferPointer->Pos = transform * s_SquareData.SquareVertexPosData[i];
			s_SquareData.pQuadVertexBufferPointer->TexCoord = s_SquareData.SquareVertexTexCoordData[i];
			s_SquareData.pQuadVertexBufferPointer->Color = whiteColor;
			s_SquareData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_SquareData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_SquareData.pQuadVertexBufferPointer->nEntityId = nEntityId;
			s_SquareData.pQuadVertexBufferPointer++;
		}

		s_SquareData.uiQuadIndexCount += 6;

		s_SquareData.Stat.uiQuadCount++;
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

		if (s_SquareData.uiQuadIndexCount >= s_SquareData.uiMaxIndices)
		{
			NextBatch();
		}

		auto texture = subTexture->GetTexture();
		auto texCoords = subTexture->GetTexCoords();

		int nFindSlot = 0;
		for (UINT i = 1; i < s_SquareData.uiTextureSlotIndex; ++i)
		{
			if (s_SquareData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_SquareData.uiTextureSlotIndex;
			s_SquareData.TextureSlots[s_SquareData.uiTextureSlotIndex] = texture;
			s_SquareData.uiTextureSlotIndex++;
		}

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));
		auto transform = translateMat * scaleMat;

		for (int i = 0; i < 4; ++i)
		{
			s_SquareData.pQuadVertexBufferPointer->Pos = transform * s_SquareData.SquareVertexPosData[i];
			s_SquareData.pQuadVertexBufferPointer->TexCoord = texCoords[i];
			s_SquareData.pQuadVertexBufferPointer->Color = whiteColor;
			s_SquareData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_SquareData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_SquareData.pQuadVertexBufferPointer++;
		}

		s_SquareData.uiQuadIndexCount += 6;

		s_SquareData.Stat.uiQuadCount++;
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

		if (s_SquareData.uiQuadIndexCount >= s_SquareData.uiMaxIndices)
		{
			NextBatch();
		}

		constexpr float fWhiteTextureSlot = 0.f;
		constexpr float fPureColorTilingFactor = 1.f;

		for (int i = 0; i < 4; ++i)
		{
			s_SquareData.pQuadVertexBufferPointer->Pos = transform * s_SquareData.SquareVertexPosData[i];
			s_SquareData.pQuadVertexBufferPointer->TexCoord = s_SquareData.SquareVertexTexCoordData[i];
			s_SquareData.pQuadVertexBufferPointer->Color = color;
			s_SquareData.pQuadVertexBufferPointer->fTexIndex = fWhiteTextureSlot;
			s_SquareData.pQuadVertexBufferPointer->fTilingFactor = fPureColorTilingFactor;
			s_SquareData.pQuadVertexBufferPointer++;
		}

		s_SquareData.uiQuadIndexCount += 6;

		s_SquareData.Stat.uiQuadCount++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		DrawRotateQuad(glm::vec3(pos2, 0.f), size, fRotationRad, texture, nTileCoef, tintColor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		PROFILE_FUNCTION();

		if (s_SquareData.uiQuadIndexCount >= s_SquareData.uiMaxIndices)
		{
			NextBatch();
		}

		int nFindSlot = 0;
		for (UINT i = 1; i < s_SquareData.uiTextureSlotIndex; ++i)
		{
			if (s_SquareData.TextureSlots[i]->GetId() == texture->GetId())
			{
				nFindSlot = i;
				break;
			}
		}
		if (nFindSlot == 0)
		{
			nFindSlot = s_SquareData.uiTextureSlotIndex;
			s_SquareData.TextureSlots[s_SquareData.uiTextureSlotIndex] = texture;
			s_SquareData.uiTextureSlotIndex++;
		}

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto rotateMat = glm::rotate(glm::mat4(1.f), fRotationRad, glm::vec3(0.f, 0.f, 1.f));
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));

		auto transform = translateMat * rotateMat * scaleMat;

		constexpr glm::vec4 whiteColor = glm::vec4(1.f);

		for (int i = 0; i < 4; ++i)
		{
			s_SquareData.pQuadVertexBufferPointer->Pos = transform * s_SquareData.SquareVertexPosData[i];
			s_SquareData.pQuadVertexBufferPointer->TexCoord = s_SquareData.SquareVertexTexCoordData[i];
			s_SquareData.pQuadVertexBufferPointer->Color = whiteColor;
			s_SquareData.pQuadVertexBufferPointer->fTexIndex = (float)nFindSlot;
			s_SquareData.pQuadVertexBufferPointer->fTilingFactor = (float)nTileCoef;
			s_SquareData.pQuadVertexBufferPointer++;
		}

		s_SquareData.uiQuadIndexCount += 6;

		s_SquareData.Stat.uiQuadCount++;
	}
	void Renderer2D::StartBatch()
	{
		s_SquareData.uiQuadIndexCount = 0;
		s_SquareData.pQuadVertexBufferPointer = s_SquareData.pQuadVertexBufferBase;

		s_SquareData.uiTextureSlotIndex = 1; //0已被白色贴图占据
	}
	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
}
