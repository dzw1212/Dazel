#include "DazelPCH.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace DAZEL
{

	static Renderer2D::Renderer2DData* s_SquareData;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();

		s_SquareData = new Renderer2D::Renderer2DData();

		s_SquareData->SquareVertexArray = VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, 0.5f, 0.f, 0.f, 1.f,
			0.5f, 0.5f, 0.f, 1.f, 1.f,
			0.5f, -0.5f, 0.f, 1.f, 0.f, 
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
		};

		BufferLayout squareLayout = {
			{ShaderDataType::VEC3, "vertPos"},
			{ShaderDataType::VEC2, "texCoord"},
		};

		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout(squareLayout);
		s_SquareData->SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		UINT squareIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(UINT));
		s_SquareData->SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_SquareData->TextureShader = Shader::Create("assert/shader/TextureShader.glsl");

		s_SquareData->WhiteTexture = Texture2D::Create(1, 1);
		UINT whiteColor = 0xffffffff;
		s_SquareData->WhiteTexture->SetData(&whiteColor, sizeof(whiteColor));
	}
	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();

		delete s_SquareData;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		s_SquareData->TextureShader->Bind();
		s_SquareData->TextureShader->SetMat4("u_ViewProjMat", camera.GetViewProjMatrix());
	}
	void Renderer2D::EndScene()
	{
		PROFILE_FUNCTION();
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

		s_SquareData->WhiteTexture->Bind();

		s_SquareData->TextureShader->Bind();
		s_SquareData->TextureShader->SetInt("u_Texture", 0);
		s_SquareData->TextureShader->SetFloat3("u_Color", color);
		s_SquareData->TextureShader->SetMat4("u_ModelMat", translateMat * scaleMat);
		s_SquareData->TextureShader->SetInt("u_TileCoef", 1);

		s_SquareData->SquareVertexArray->Bind();
		RendererCommand::DrawIndexed(s_SquareData->SquareVertexArray);
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

		texture->Bind(0);

		s_SquareData->TextureShader->Bind();
		s_SquareData->TextureShader->SetInt("u_Texture", 0);
		s_SquareData->TextureShader->SetFloat3("u_Color", tintColor);
		s_SquareData->TextureShader->SetMat4("u_ModelMat", translateMat * scaleMat);
		s_SquareData->TextureShader->SetInt("u_TileCoef", nTileCoef);

		s_SquareData->SquareVertexArray->Bind();
		RendererCommand::DrawIndexed(s_SquareData->SquareVertexArray);
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

		s_SquareData->WhiteTexture->Bind();

		s_SquareData->TextureShader->Bind();
		s_SquareData->TextureShader->SetInt("u_Texture", 0);
		s_SquareData->TextureShader->SetFloat3("u_Color", color);
		s_SquareData->TextureShader->SetMat4("u_ModelMat", translateMat * rotateMat * scaleMat);
		s_SquareData->TextureShader->SetInt("u_TileCoef", 1);

		s_SquareData->SquareVertexArray->Bind();
		RendererCommand::DrawIndexed(s_SquareData->SquareVertexArray);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& pos2, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		DrawRotateQuad(glm::vec3(pos2, 0.f), size, fRotationRad, texture, nTileCoef, tintColor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& pos3, const glm::vec2& size, float fRotationRad, const Ref<Texture2D>& texture, int nTileCoef, const glm::vec3& tintColor)
	{
		PROFILE_FUNCTION();

		auto translateMat = glm::translate(glm::mat4(1.f), pos3);
		auto rotateMat = glm::rotate(glm::mat4(1.f), fRotationRad, glm::vec3(0.f, 0.f, 1.f));
		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));

		texture->Bind(0);

		s_SquareData->TextureShader->Bind();
		s_SquareData->TextureShader->SetInt("u_Texture", 0);
		s_SquareData->TextureShader->SetFloat3("u_Color", tintColor);
		s_SquareData->TextureShader->SetMat4("u_ModelMat", translateMat * rotateMat * scaleMat);
		s_SquareData->TextureShader->SetInt("u_TileCoef", nTileCoef);

		s_SquareData->SquareVertexArray->Bind();
		RendererCommand::DrawIndexed(s_SquareData->SquareVertexArray);
	}
}