#pragma once

#include "Dazel/Core/Layer.h"
#include "Dazel/Renderer/VertexArray.h"
#include "Dazel/Renderer/Texture.h"
#include "Dazel/Renderer/Shader.h"
#include "Dazel/Camera/OrthographicCameraController.h"

class Sandbox2D : public DAZEL::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(DAZEL::Timestep timeStep) override;
	virtual void OnEvent(DAZEL::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	DAZEL::ShaderLibrary m_ShaderLibrary;
	DAZEL::Ref<DAZEL::VertexArray> m_SquareVertexArray;
	DAZEL::OrthographicCameraController m_CameraController;
	DAZEL::Ref<DAZEL::Texture2D> m_Texture;
	int m_nTextureTileCoef = 1;
	glm::vec3 m_SquareColor = glm::vec3(1.f);
};