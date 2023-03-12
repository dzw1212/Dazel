#include "Dazel.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

///////////////////////Entry Point////////////////////////
#include "Dazel/Core/EntryPoint.h" //注意include顺序，EntryPoint需要置后

#include "Sandbox2D.h"

class ExampleLayer : public DAZEL::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer"), m_CameraController(1.f, true)
	{

		/*m_VertexArray.reset(DAZEL::VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.f,		1.f, 0.f, 0.f, 1.f,
			0.f, 0.5f, 0.f,			0.f, 1.f, 0.f, 1.f,
			0.5f, -0.5f, 0.f,		0.f, 0.f, 1.f, 1.f,
		};

		m_SquareVertexArray.reset(DAZEL::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, 0.5f, 0.f, 0.f, 1.f,
			0.5f, 0.5f, 0.f, 1.f, 1.f,
			0.5f, -0.5f, 0.f, 1.f, 0.f,
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
		};

		DAZEL::BufferLayout layout = {
			{DAZEL::ShaderDataType::VEC3, "vertPos"},
			{DAZEL::ShaderDataType::VEC4, "color"},
		};

		DAZEL::BufferLayout squareLayout = {
			{DAZEL::ShaderDataType::VEC3, "vertPos"},
			{DAZEL::ShaderDataType::VEC2, "texCoord"},
		};

		DAZEL::Ref<DAZEL::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DAZEL::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		DAZEL::Ref<DAZEL::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(DAZEL::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);


		UINT indices[3] = {
			0, 2, 1,
		};

		UINT squareIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		DAZEL::Ref<DAZEL::IndexBuffer> indexBuffer;
		indexBuffer.reset(DAZEL::IndexBuffer::Create(indices, sizeof(indices)/sizeof(UINT)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		DAZEL::Ref<DAZEL::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(DAZEL::IndexBuffer::Create(squareIndices, sizeof(squareIndices)/sizeof(UINT)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);
		
		m_ShaderLibrary.Load("assert/shader/triangleShader.glsl");
		m_ShaderLibrary.Load("assert/shader/squareShader.glsl");
		m_ShaderLibrary.Load("assert/shader/textureShader.glsl");
	
		m_Texture = DAZEL::Texture2D::Create("assert/texture/icon.png");
		m_Texture2 = DAZEL::Texture2D::Create("assert/texture/icon4.png");*/
	}

	void OnUpdate(DAZEL::Timestep timeStep) override
	{
		//m_CameraController.OnUpdate(timeStep);

		//DAZEL::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.f));
		//DAZEL::RendererCommand::Clear();

		//DAZEL::Renderer::BeginScene(m_CameraController.GetCamera());

		////for (float i = 0.f; i < 15.f; ++i)
		////{
		////	for (float j = 0.f; j < 15.f; ++j)
		////	{
		////		auto translateMat = glm::translate(glm::mat4(1.f), glm::vec3(i * 0.11f, j * 0.11f, 0.f));
		////		auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));
		////		std::dynamic_pointer_cast<DAZEL::OpenGLShader>(m_SquareShader)->UploadUniformfVec3("u_Color", glm::vec3(i/15.f, j/15.f, m_TestB));
		////		DAZEL::Renderer::Submit(m_SquareVertexArray, m_SquareShader, translateMat * scaleMat);
		////	}
		////}

		//m_Texture->Bind(0);
		//std::dynamic_pointer_cast<DAZEL::OpenGLShader>(m_ShaderLibrary.Get("textureShader"))->UploadUniformIVec1("u_Texture", glm::ivec1(0));
		//DAZEL::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("textureShader"), glm::scale(glm::mat4(1.f), glm::vec3(2.f)));

		//m_Texture2->Bind(0);
		//std::dynamic_pointer_cast<DAZEL::OpenGLShader>(m_ShaderLibrary.Get("textureShader"))->UploadUniformIVec1("u_Texture", glm::ivec1(0));
		//DAZEL::Renderer::Submit(m_SquareVertexArray, m_ShaderLibrary.Get("textureShader"), glm::scale(glm::mat4(1.f), glm::vec3(2.f)));

		////DAZEL::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.f));

		//DAZEL::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		//ImGui::Begin("Pick a color");
		//ImGui::ColorEdit3("ColorPicker", glm::value_ptr(m_SquareColor));
		//ImGui::SliderFloat("ColorPicker", &m_TestB, 0.f, 1.f);
		//ImGui::End();
	}

	void OnEvent(DAZEL::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:
	DAZEL::ShaderLibrary m_ShaderLibrary;

	DAZEL::Ref<DAZEL::VertexArray> m_VertexArray;

	DAZEL::Ref<DAZEL::VertexArray> m_SquareVertexArray;

	DAZEL::OrthographicCameraController m_CameraController;

	DAZEL::Ref<DAZEL::Texture2D> m_Texture, m_Texture2;
};

class SandBox : public DAZEL::Application
{
public:
	SandBox()
		: Application("Sandbox")
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	} 

	~SandBox()
	{

	}


};

DAZEL::Application* DAZEL::CreateApplication()
{
	return new SandBox();
}