#include "Sandbox2D.h"

#include "Dazel.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <chrono>

glm::vec2 CurMousePosTo2DPos(const DAZEL::OrthographicCameraController& cameraController)
{
	auto [x, y] = DAZEL::Input::GetMousePos();
	auto width = DAZEL::Application::Get().GetWindow().GetWidth();
	auto height = DAZEL::Application::Get().GetWindow().GetHeight();
	auto bounds = cameraController.GetBounds();
	auto cameraPos = cameraController.GetCamera().GetPosition();
	x = (x / width) * bounds.GetWidth() - bounds.GetWidth() / 2.f;
	y = bounds.GetHeight() / 2.f - (y / height) * bounds.GetHeight();

	return { x + cameraPos.x, y + cameraPos.y };
}


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2DLayer"), m_CameraController(1280.f / 960.f, true)
{

}

void Sandbox2D::OnAttach()
{
	//m_ShaderLibrary.Load("assert/shader/SquareShader.glsl");

	m_Texture = DAZEL::Texture2D::Create("assert/texture/icon.png");
	m_SpriteSheet = DAZEL::Texture2D::Create("assert/game/texture/tilemap_packed.png");

	m_CameraController.SetZoomLevel(5.f);

	m_ParticleProps.ColorBegin = glm::vec4(254 / 255.f, 212 / 255.f, 123 / 255.f, 1.f);
	m_ParticleProps.ColorEnd = glm::vec4(254 / 255.f, 109 / 255.f, 41 / 255.f, 1.f);
	m_ParticleProps.fSizeBegin = 0.5;
	m_ParticleProps.fSizeEnd = 0.f;
	m_ParticleProps.fSizeVariation = 0.3;
	m_ParticleProps.fLifeTime = 1.f;
	m_ParticleProps.Velocity = glm::vec2(0.f);
	m_ParticleProps.VelocityVariation = glm::vec2(3.f, 1.f);
	m_ParticleProps.Position = glm::vec2(0.f);
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
		//float fMax = std::max(m_SpriteSheet->GetWidth(), m_SpriteSheet->GetHeight());
		//glm::vec2 size = { m_SpriteSheet->GetWidth() / fMax, m_SpriteSheet->GetHeight() / fMax };
		//DAZEL::Renderer2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1), size, m_SpriteSheet);
		

		//auto subTexture = DAZEL::SubTexture2D::CreateByCoord(m_SpriteSheet, { 4, 4 }, { 18, 18 }, {1, 2});
		//DAZEL::Renderer2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1), {1, 2}, subTexture);

		glm::vec3 pos1 = glm::vec3(0.f, 3.f, 0.f);

		auto trans1 = glm::translate(glm::mat4(1.f), pos1);

		DAZEL::Renderer2D::DrawQuad(trans1, glm::vec4(1.f, 0.f, 0.f, 1.f));

		//for (float x = -5.f; x < 5.f; x += 0.5)
		//{
		//	for (float y = -5.f; y < 5.f; y += 0.5)
		//	{
		//		DAZEL::Renderer2D::DrawRotateQuad(glm::vec3(x, y, -0.1), glm::vec2(0.45, 0.45), glm::radians(fAngle), glm::vec4((x + 5.f) / 10.f, (y + 5.f) / 10.f, 1.f, 1.f));
		//	}
		//}


		DAZEL::Renderer2D::EndScene();
		
		//if (DAZEL::Input::IsMouseButtonPressed(DAZEL_MOUSE_BUTTON_LEFT))
		//{
		//	m_ParticleProps.Position = CurMousePosTo2DPos(m_CameraController);

		//	for (int i = 0; i < 5; ++i)
		//		m_ParticleSystem.Emit(m_ParticleProps);
		//}

		//m_ParticleSystem.OnUpdate(timeStep);
		//m_ParticleSystem.OnRender(m_CameraController.GetCamera());
		
		//fAngle += timeStep.GetSeconds() * 10;


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
