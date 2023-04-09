
#include "Dazel/Core/KeyCodes.h"

#include "Platform/Windows/WindowsPlatformUtils.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "EditorLayer.h"
#include "Dazel/Scene/Components.h"

glm::vec2 CurMousePosTo2DPos(const glm::vec2& mousePos, const glm::vec2& viewPortSize, 
	const glm::vec2& bounds, const glm::vec3& cameraPos)
{
	float x = mousePos.x;
	float y = mousePos.y;
	auto width = viewPortSize.x;
	auto height = viewPortSize.y;
	x = (x / width) * bounds.x - bounds.x / 2.f;
	y = bounds.y / 2.f - (y / height) * bounds.y;
	return { x + cameraPos.x, y + cameraPos.y };
}


EditorLayer::EditorLayer()
	: Layer("EditorLayer"), 
	m_CameraController(1280.f / 960.f, true),
	m_EditorCamera(30.f, 1280.f / 960.f, 0.1f, 1000.f)
{

}

void EditorLayer::OnAttach()
{
	//Textures
	//m_ShaderLibrary.Load("assert/shader/SquareShader.glsl");
	m_Texture = DAZEL::Texture2D::Create("assert/texture/icon.png");
	m_SpriteSheet = DAZEL::Texture2D::Create("assert/game/texture/tilemap_packed.png");

	m_PlayIcon = DAZEL::Texture2D::Create("Resource/Icon/Scene/PlayIcon.png");
	m_StopIcon = DAZEL::Texture2D::Create("Resource/Icon/Scene/StopIcon.png");


	//CameraComtroller
	m_CameraController.SetZoomLevel(5.f);

	//Partical System
	m_ParticleProps.ColorBegin = glm::vec4(254 / 255.f, 212 / 255.f, 123 / 255.f, 1.f);
	m_ParticleProps.ColorEnd = glm::vec4(254 / 255.f, 109 / 255.f, 41 / 255.f, 1.f);
	m_ParticleProps.fSizeBegin = 0.5;
	m_ParticleProps.fSizeEnd = 0.f;
	m_ParticleProps.fSizeVariation = 0.3f;
	m_ParticleProps.fLifeTime = 1.f;
	m_ParticleProps.Velocity = glm::vec2(0.f);
	m_ParticleProps.VelocityVariation = glm::vec2(3.f, 1.f);
	m_ParticleProps.Position = glm::vec2(0.f);

	//Frame Buffer
	DAZEL::FrameBufferSpecification frameBufferSpec;
	frameBufferSpec.uiWidth = DAZEL::Application::Get().GetWindow().GetWidth();
	frameBufferSpec.uiHeight = DAZEL::Application::Get().GetWindow().GetHeight();
	frameBufferSpec.AttachmentSpec = {
		DAZEL::FrameBufferTextureFormat::DEFAULT_COLOR,
		DAZEL::FrameBufferTextureFormat::RED_INTEGER,	//用于MousePick
		DAZEL::FrameBufferTextureFormat::DEFAULT_DEPTH };
	m_FrameBuffer = DAZEL::FrameBuffer::Create(frameBufferSpec);

	//Scene
	m_EditorScene = DAZEL::CreateRef<DAZEL::Scene>();
	m_ActiveScene = m_EditorScene;

	auto commandLineArgs = DAZEL::Application::Get().GetCommandLineArgs();
	if (commandLineArgs.nCount > 1)
	{
		auto sceneFilePath = commandLineArgs[1];
		DAZEL::SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(sceneFilePath);
	}

#if 0
	m_SquareEntity = m_ActiveScene->CreateEntity("Square");

	auto translateMat = glm::translate(glm::mat4(1.f), glm::vec3(0.f));
	auto rotateMat = glm::rotate(glm::mat4(1.f), 0.f, glm::vec3(0.f, 0.f, 1.f));
	auto scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
	auto transform = translateMat * rotateMat * scaleMat;

	m_SquareEntity.AddComponent<DAZEL::SpriteRendererComponent>(glm::vec4(1.f, 15.f/255.f, 0.f, 1.f));
	m_SquareEntity.GetComponent<DAZEL::TransformComponent>().m_Position = {0.f, 3.f, 0.f};

	m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
	m_CameraEntity.AddComponent<DAZEL::CameraComponent>().m_bIsMainCamera = true;

	//m_ClipSapceCameraEntity = m_ActiveScene->CreateEntity("Clip Space Camera");
	//m_ClipSapceCameraEntity.AddComponent<DAZEL::CameraComponent>(glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f)).m_bIsMainCamera = false;
#endif

	m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	m_SceneSerializer.SetScene(m_ActiveScene);
}

void EditorLayer::OnDetach()
{
}

float fAngle = 0.f;
void EditorLayer::OnUpdate(DAZEL::Timestep timeStep)
{
	PROFILE_FUNCTION();

	{
		PROFILE_SCOPE("Resize");

		auto spec = m_FrameBuffer->GetSpec();
		if (m_ViewportPanelSize.x > 0.f && m_ViewportPanelSize.y > 0.f 
			&& (spec.uiWidth != (UINT)m_ViewportPanelSize.x || spec.uiHeight != (UINT)m_ViewportPanelSize.y))
		{
			m_FrameBuffer->Resize((UINT)m_ViewportPanelSize.x, (UINT)m_ViewportPanelSize.y);
			m_CameraController.SetAspectRatio(m_ViewportPanelSize.x / m_ViewportPanelSize.y);

			m_ActiveScene->OnViewportResize((UINT)m_ViewportPanelSize.x, (UINT)m_ViewportPanelSize.y);
		
			m_EditorCamera.SetViewportSize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
		}
	}

	{
		PROFILE_SCOPE("Prepare Scene");

		m_FrameBuffer->Bind();

		DAZEL::RendererCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.f));
		DAZEL::RendererCommand::Clear();

		m_FrameBuffer->ClearColorAttachment(1, -1);
	}

	DAZEL::Renderer2D::ResetStatisticData();

	{
		PROFILE_SCOPE("Render Scene");

		//DAZEL::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//float fMax = std::max(m_SpriteSheet->GetWidth(), m_SpriteSheet->GetHeight());
		//glm::vec2 size = { m_SpriteSheet->GetWidth() / fMax, m_SpriteSheet->GetHeight() / fMax };
		//DAZEL::Renderer2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1), size, m_SpriteSheet);

		//auto subTexture = DAZEL::SubTexture2D::CreateByCoord(m_SpriteSheet, { 4, 4 }, { 18, 18 }, {1, 2});
		//DAZEL::Renderer2D::DrawQuad(glm::vec3(0.f, 0.f, -0.1), {1, 2}, subTexture);

		//DAZEL::Renderer2D::DrawQuad(glm::vec2(0.f, 0.f), glm::vec2(5.f, 5.f), glm::vec4(1.f, 0.f, 0.f, 1.f));
			

		switch (m_SceneState)
		{
			case SceneState::PLAY:
			{
				m_ActiveScene->OnUpdateRuntime(timeStep);
				break;
			}
			case SceneState::EDIT:
			{
				m_EditorCamera.OnUpdate(timeStep);
				m_ActiveScene->OnUpdateEditor(timeStep, m_EditorCamera);
				//if (m_bIsViewportPanelFocused)
				//{
				//	m_CameraController.OnUpdate(timeStep);
				//}
				break;
			}
		}
		
		//if (DAZEL::Input::IsMouseButtonPressed(DAZEL_MOUSE_BUTTON_LEFT))
		//{
		//	m_ParticleProps.Position = CurMousePosTo2DPos(m_CameraController);

		//	for (int i = 0; i < 5; ++i)
		//		m_ParticleSystem.Emit(m_ParticleProps);
		//}

		//m_ParticleSystem.OnUpdate(timeStep);
		//m_ParticleSystem.OnRender(m_CameraController.GetCamera());
		
		//fAngle += timeStep.GetSeconds() * 10;

		m_FrameBuffer->Unbind();
	}
}

void EditorLayer::OnEvent(DAZEL::Event& event)
{
	m_CameraController.OnEvent(event);

	DAZEL::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<DAZEL::KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	dispatcher.Dispatch<DAZEL::MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMousePressed));
}

void EditorLayer::OnImGuiRender()
{
	PROFILE_FUNCTION();

	static bool dockingSpaceOpen = true;
	if (dockingSpaceOpen)
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockingSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 600.f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = 32.f;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Exit"))
					DAZEL::Application::Get().Close();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	ImGui::Begin("Setting:");

	auto RendererStata = DAZEL::Renderer2D::GetStatisticData();
	ImGui::Text("Renderer2D statistics:");
	ImGui::Text("Quad count:%d", RendererStata.uiQuadCount);
	ImGui::Text("Draw call:%d", RendererStata.uiDrawCalls);

	ImGui::Separator();

	ImGui::Text("Viewport MousePos: (%.2f, %.2f)", m_ViewportRegionMousePos.x, m_ViewportRegionMousePos.y);
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f)); //去掉黑色边框，别忘了Pop
	ImGui::Begin("Viewport:");

	auto inViewportPanelOffset = ImGui::GetCursorPos();

	m_bIsViewportPanelFocused = ImGui::IsWindowFocused();
	m_bIsViewportPanelHovered = ImGui::IsWindowHovered();
	DAZEL::Application::Get().GetImGuiLayer()->BlockEvents(!m_bIsViewportPanelHovered);
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if ((m_ViewportPanelSize.x != viewportPanelSize.x) || (m_ViewportPanelSize.y != viewportPanelSize.y))
	{
		m_ViewportPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
	}
	ImGui::Image((ImTextureID)(m_FrameBuffer->GetColorAttachmentId(0)), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ASSERT_PATH"))
		{
			auto data = (const char*)payload->Data;
			auto path = std::filesystem::path(data);
			auto fileExtension = path.filename().extension();
			if (fileExtension == ".dazel")
			{
				OpenScene(path);
			}
			else if (fileExtension == ".png")
			{
				if (m_MouseHoverEntity != -1)
				{
					auto& sprite = m_MouseHoverEntity.GetComponent<DAZEL::SpriteRendererComponent>();
					sprite.m_Texture = DAZEL::Texture2D::Create(path.string());
				}
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImVec2 minBound = ImGui::GetWindowPos();

	minBound.x += inViewportPanelOffset.x;
	minBound.y += inViewportPanelOffset.y;
	ImVec2 maxBound = { minBound.x + m_ViewportPanelSize.x, minBound.y + m_ViewportPanelSize.y };
	m_ViewportRegionPos[0] = { minBound.x, minBound.y };
	m_ViewportRegionPos[1] = { maxBound.x, maxBound.y };

	auto mouseGlobalPos = ImGui::GetMousePos();
	m_bIsMouseInViewport = (mouseGlobalPos.x >= m_ViewportRegionPos[0].x)
		&& (mouseGlobalPos.y >= m_ViewportRegionPos[0].y) 
		&& (mouseGlobalPos.x <= m_ViewportRegionPos[1].x) 
		&& (mouseGlobalPos.y <= m_ViewportRegionPos[1].y);

	if (m_bIsMouseInViewport)
	{
		m_ViewportRegionMousePos = { mouseGlobalPos.x - m_ViewportRegionPos[0].x, 
			m_ViewportPanelSize.y - (mouseGlobalPos.y - m_ViewportRegionPos[0].y) }; //y坐标修正
		int nMouseHoverEntityId = m_FrameBuffer->ReadPixel(1, (int)m_ViewportRegionMousePos.x, (int)m_ViewportRegionMousePos.y);
		m_MouseHoverEntity = nMouseHoverEntityId == -1 ? DAZEL::Entity() : DAZEL::Entity((entt::entity)nMouseHoverEntityId, m_ActiveScene.get());
	}

	//Gizmos
	DAZEL::Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
	if (selectedEntity)
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		float fWindowWidth = (float)ImGui::GetWindowWidth();
		float fWindowHeight = (float)ImGui::GetWindowHeight();
		auto windowPos = ImGui::GetWindowPos();
		ImGuizmo::SetRect(windowPos.x, windowPos.y, fWindowWidth, fWindowHeight);

		if (m_SceneState == SceneState::EDIT)
		{
			auto viewMat = m_EditorCamera.GetViewMatrix();
			auto projectionMat = m_EditorCamera.GetProjection();

			auto& selectedTransformComponent = selectedEntity.GetComponent<DAZEL::TransformComponent>();
			auto transformMat = selectedTransformComponent.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat), m_curGizmoOperation, m_curGizmoMode, glm::value_ptr(transformMat));

			if (ImGuizmo::IsUsing)
			{
				float matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformMat), matrixTranslation, matrixRotation, matrixScale);

				selectedTransformComponent.m_Position = { matrixTranslation[0], matrixTranslation[1], matrixTranslation[2] };
				selectedTransformComponent.m_Rotation = glm::radians(glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));
				selectedTransformComponent.m_Scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
			}
		}
		else
		{
			auto mainCameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			if (mainCameraEntity)
			{
				auto& cameraComponent = mainCameraEntity.GetComponent<DAZEL::CameraComponent>();
				auto& transformComponent = mainCameraEntity.GetComponent<DAZEL::TransformComponent>();
				auto viewMat = glm::inverse(transformComponent.GetTransform());
				auto projectionMat = cameraComponent.m_Camera.GetProjection();

				auto& selectedTransformComponent = selectedEntity.GetComponent<DAZEL::TransformComponent>();
				auto transformMat = selectedTransformComponent.GetTransform();

				ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat), m_curGizmoOperation, m_curGizmoMode, glm::value_ptr(transformMat));

				if (ImGuizmo::IsUsing)
				{
					float matrixTranslation[3], matrixRotation[3], matrixScale[3];
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformMat), matrixTranslation, matrixRotation, matrixScale);

					selectedTransformComponent.m_Position = { matrixTranslation[0], matrixTranslation[1], matrixTranslation[2] };
					selectedTransformComponent.m_Rotation = glm::radians(glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));
					selectedTransformComponent.m_Scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
				}
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	m_SceneHierarchyPanel.OnImGuiRender();
	m_ContentBrowserPanel.OnImGuiRender();

	UI_Tools();
}

void EditorLayer::NewScene()
{
	if (m_SceneState != SceneState::EDIT)
		return;
	m_EditorScene = DAZEL::CreateRef<DAZEL::Scene>();
	m_ActiveScene = m_EditorScene;

	m_ActiveScene->OnViewportResize((UINT)m_ViewportPanelSize.x, (UINT)m_ViewportPanelSize.y);
	m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	m_CurrentSceneFilePath.clear();
}

void EditorLayer::OpenScene()
{
	if (m_SceneState != SceneState::EDIT)
		OnSceneStop();

	auto strFilepath = DAZEL::FileDialogs::OpenFile("Dazel Scene (*.dazel)\0*.dazel\0");
	OpenScene(strFilepath);
}

void EditorLayer::OpenScene(const std::filesystem::path& path)
{
	if (m_SceneState != SceneState::EDIT)
		OnSceneStop();

	auto strFilePath = path.string();
	if (!strFilePath.empty())
	{
		m_EditorScene = DAZEL::CreateRef<DAZEL::Scene>();
		m_ActiveScene = m_EditorScene;

		m_ActiveScene->OnViewportResize((UINT)m_ViewportPanelSize.x, (UINT)m_ViewportPanelSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_SceneSerializer.SetScene(m_ActiveScene);
		m_SceneSerializer.Deserialize(strFilePath);

		m_CurrentSceneFilePath = strFilePath;
	}
}

void EditorLayer::SaveScene()
{
	if (m_CurrentSceneFilePath.empty())
	{
		SaveSceneAs();
	}
	else
		m_SceneSerializer.Serialize(m_CurrentSceneFilePath.string());
	
}

void EditorLayer::SaveSceneAs()
{
	auto strFilepath = DAZEL::FileDialogs::SaveFile("Dazel Scene (*.dazel)\0*.dazel\0");
	if (!strFilepath.empty())
	{
		m_SceneSerializer.Serialize(strFilepath);
		m_CurrentSceneFilePath = strFilepath;
	}
}

void EditorLayer::UI_Tools()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2)); //窗口填充，x轴0像素，y轴2像素
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0)); //内部间距
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(15.f / 255.f, 15.f / 255.f, 15.f / 255.f, 1.f)); //按钮的颜色

	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##ToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	bool toolbarEnabled = (bool)m_ActiveScene;

	ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
	if (!toolbarEnabled)
		tintColor.w = 0.5f;

	float fSize = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (fSize * 0.5f)); //居中

	auto icon = m_SceneState == SceneState::PLAY ? m_StopIcon : m_PlayIcon;

	if (ImGui::ImageButton((ImTextureID)icon->GetId(), { fSize, fSize }))
	{
		if (m_SceneState == SceneState::PLAY)
			OnSceneStop();
		else if (m_SceneState == SceneState::EDIT)
			OnScenePlay();
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

	ImGui::End();
}

void EditorLayer::OnScenePlay()
{
	m_SceneState = SceneState::PLAY;
	m_RuntimeScene = DAZEL::Scene::Copy(m_EditorScene);

	m_ActiveScene = m_RuntimeScene;
	m_SceneHierarchyPanel.SetSelectedEntity({});

	m_ActiveScene->OnRuntimeStart();
}

void EditorLayer::OnSceneStop()
{
	m_SceneState = SceneState::EDIT;
	m_ActiveScene = m_EditorScene;
	m_ActiveScene->OnRuntimeStop();
}

bool EditorLayer::OnKeyPressed(DAZEL::KeyPressedEvent& event)
{
	if (event.GetRepeatCount() > 0)
		return false;

	if (DAZEL::Input::IsKeyPressed(DAZEL_KEY_LEFT_CONTROL) || DAZEL::Input::IsKeyPressed(DAZEL_KEY_RIGHT_CONTROL))
	{
		switch (event.GetKeyCode())
		{
		case DAZEL_KEY_N:
		{
			NewScene();
			break;
		}
		case DAZEL_KEY_O:
		{
			OpenScene();
			break;
		}
		case DAZEL_KEY_S:
		{
			SaveScene();
			break;
		}
		case DAZEL_KEY_D:
		{
			m_ActiveScene->CopyEntity(m_SceneHierarchyPanel.GetSelectedEntity());
			break;
		}
		default:
			break;
		}
	}

	switch (event.GetKeyCode())
	{
	case DAZEL_KEY_T:
	{
		m_curGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		break;
	}
	case DAZEL_KEY_R:
	{
		m_curGizmoOperation = ImGuizmo::OPERATION::ROTATE;
		break;
	}
	case DAZEL_KEY_S:
	{
		m_curGizmoOperation = ImGuizmo::OPERATION::SCALE;
		break;
	}
	}
	

	return false;
}

bool EditorLayer::OnMousePressed(DAZEL::MouseButtonPressedEvent& event)
{
	if (m_SceneState != SceneState::EDIT)
		return false;

	if (event.GetMouseButton() == DAZEL_MOUSE_BUTTON_LEFT)
	{
		if (m_bIsMouseInViewport
			&& !ImGuizmo::IsOver()
			&& !DAZEL::Input::IsKeyPressed(DAZEL_KEY_LEFT_CONTROL))
		{
			m_SceneHierarchyPanel.SetSelectedEntity(m_MouseHoverEntity);
		}
	}
	return false;
}
