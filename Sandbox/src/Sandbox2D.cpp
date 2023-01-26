#include "Sandbox2D.h"

#include "Dazel.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <chrono>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2DLayer"), m_CameraController(1280.f / 960.f, true)
{

}

void Sandbox2D::OnAttach()
{
	//m_ShaderLibrary.Load("assert/shader/SquareShader.glsl");

	m_Texture = DAZEL::Texture2D::Create("assert/texture/icon.png");
}

void Sandbox2D::OnDetach()
{
}

float fAngle = 0.f;
void Sandbox2D::OnUpdate(DAZEL::Timestep timeStep)
{
	PROFILE_FUNCTION();
	{
		PROFILE_SCOPE("CameraController");
		m_CameraController.OnUpdate(timeStep);
	}

	{
		PROFILE_SCOPE("Prepare Scene");
		DAZEL::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.f));
		DAZEL::RendererCommand::Clear();
	}

	DAZEL::Renderer2D::ResetStatisticData();

	{
		PROFILE_SCOPE("Render Scene");

		DAZEL::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//DAZEL::Renderer2D::DrawRotateQuad(glm::vec3(-0.75, 0.f, -0.5), glm::vec2(15.f), glm::radians(45.f), m_Texture, m_nTextureTileCoef, glm::vec3(1.f, 1.f, 0.f));
		DAZEL::Renderer2D::DrawQuad(glm::vec2(0.75, 0.f), glm::vec2(0.25, 0.75), glm::vec4(m_SquareColor, 1.f));
		DAZEL::Renderer2D::DrawRotateQuad(glm::vec2(0.f, 0.f), glm::vec2(0.1f, 0.1f), glm::radians(45.f), glm::vec4(1.f, 1.f, 1.f, 1.f));

		//DAZEL::Renderer2D::DrawRotateQuad(glm::vec3(0.f, 0.f, -0.1), glm::vec2(2.f, 2.f), glm::radians(45.f), m_Texture, m_nTextureTileCoef);

		
		fAngle += timeStep.GetSeconds() * 10;

		for (float x = -5.f; x < 5.f; x += 0.5)
		{
			for (float y = -5.f; y < 5.f; y += 0.5)
			{
				DAZEL::Renderer2D::DrawRotateQuad(glm::vec3(x, y, -0.1), glm::vec2(0.45, 0.45), glm::radians(fAngle), glm::vec4((x+5.f)/10.f, (y+5.f)/10.f, 1.f, 1.f));
			}
		}

		DAZEL::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(DAZEL::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	PROFILE_FUNCTION();

	ImGui::Begin("Pick a color");
	ImGui::ColorEdit3("ColorPicker", glm::value_ptr(m_SquareColor));
	ImGui::SliderInt("TileCoef", &m_nTextureTileCoef, 1, 100);

	auto RendererStata = DAZEL::Renderer2D::GetStatisticData();
	ImGui::Text("Renderer2D statistics:");
	ImGui::Text("Quad count:%d", RendererStata.uiQuadCount);
	ImGui::Text("Draw call:%d", RendererStata.uiDrawCalls);

	ImGui::End();
}
