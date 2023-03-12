#pragma once

#include "Dazel.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Dazel/Camera/EditorCamera.h"

#include "ImGuizmo.h"

class EditorLayer : public DAZEL::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(DAZEL::Timestep timeStep) override;
	virtual void OnEvent(DAZEL::Event& event) override;
	virtual void OnImGuiRender() override;
public:
	void NewScene();
	void OpenScene();
	void SaveScene();
public:
	bool OnKeyPressed(DAZEL::KeyPressedEvent& event);
	bool OnMousePressed(DAZEL::MouseButtonPressedEvent& event);

private:
	DAZEL::ShaderLibrary m_ShaderLibrary;
	DAZEL::Ref<DAZEL::VertexArray> m_SquareVertexArray;
	DAZEL::OrthographicCameraController m_CameraController;
	DAZEL::Ref<DAZEL::Texture2D> m_Texture;
	DAZEL::Ref<DAZEL::Texture2D> m_SpriteSheet;
	int m_nTextureTileCoef = 1;
	glm::vec3 m_SquareColor = glm::vec3(1.f);

	DAZEL::ParticleSystem m_ParticleSystem;
	DAZEL::ParticleProps m_ParticleProps;

	DAZEL::Ref<DAZEL::FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewportPanelSize;
	glm::vec2 m_ViewportRegionPos[2]; //leftTop, rightBottom
	glm::vec2 m_ViewportRegionMousePos = {0.f, 0.f};
	DAZEL::Entity m_MouseHoverEntity;
	bool m_bIsMouseInViewport = false;

	bool m_bIsViewportPanelFocused = false;
	bool m_bIsViewportPanelHovered = false;

	DAZEL::Ref<DAZEL::Scene> m_ActiveScene;
	DAZEL::Entity m_SquareEntity;
	DAZEL::Entity m_SquareEntity2;
	DAZEL::Entity m_CameraEntity;
	DAZEL::Entity m_ClipSapceCameraEntity;

	DAZEL::SceneHierarchyPanel m_SceneHierarchyPanel;
	DAZEL::SceneSerializer m_SceneSerializer;

	//Gizmo
	ImGuizmo::OPERATION m_curGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE m_curGizmoMode = ImGuizmo::MODE::LOCAL;

	//EditorCamera
	DAZEL::EditorCamera m_EditorCamera;
	bool m_bEnableEditorCamera = false;
};