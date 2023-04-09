#include <filesystem>
#include "Dazel.h"

#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"



namespace DAZEL
{
	static void DrawVec3Control(const std::string& strLabel, glm::vec3& values, float fResetValue = 0.f, float fColumnWidth = 100.f)
	{
		auto& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(strLabel.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, fColumnWidth);
		ImGui::Text(strLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });

		float fLineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { fLineHeight + 3.f, fLineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = fResetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = fResetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = fResetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawVec2Control(const std::string& strLabel, glm::vec2& values, float fResetValue = 0.f, float fColumnWidth = 100.f)
	{
		auto& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(strLabel.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, fColumnWidth);
		ImGui::Text(strLabel.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });

		float fLineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { fLineHeight + 3.f, fLineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = fResetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = fResetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& strName, Entity entity, UIFunction func, bool bAllowedRemove = false)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_FramePadding | 
			ImGuiTreeNodeFlags_SpanAvailWidth;

		bool bRemove = false;

		ImVec2 contentRegionAvalibale = ImGui::GetContentRegionAvail();

		auto& component = entity.GetComponent<T>();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.f, 4.f });
		float fLineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		bool bOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, strName.c_str());
		if (bAllowedRemove)
		{
			ImGui::SameLine(contentRegionAvalibale.x - fLineHeight * 0.5f);
			if (ImGui::Button("...", ImVec2(fLineHeight, fLineHeight)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove"))
					bRemove = true;
				ImGui::EndPopup();
			}
		}
		ImGui::PopStyleVar();

		

		if (bOpen)
		{
			func(component);

			ImGui::TreePop();
		}

		if (bRemove)
			entity.RemoveComponent<T>();
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy Panel:");
		ImGui::Separator();
		m_Context->GetRegistry().view<TagComponent>().each(
			[&](auto entityId, auto& tagComponent) 
			{
				auto entity = Entity(entityId, m_Context.get());
				DrawEntityNode(entity);
			}
		);

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create New Entity"))
				m_Context->CreateEntity("New Entity");
			ImGui::EndPopup();
		}


		ImGui::End();

		ImGui::Begin("Components:");
		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().m_strTag;

		ImGuiTreeNodeFlags flags =
			((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_Bullet | 
			ImGuiTreeNodeFlags_SpanAvailWidth;
		
		if (ImGui::TreeNodeEx((void*)entity.GetId(), flags, tag.c_str()))
		{
			ImGui::TreePop();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				if (m_SelectedEntity == entity)
					m_SelectedEntity = {};
				m_Context->DestroyEntity(entity);
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			DrawComponent<TagComponent>("Tag", entity, [](TagComponent& component)
				{
					auto& tag = component.m_strTag;
					char buffer[256];
					memset(buffer, 0, sizeof(buffer));
					sprintf_s(buffer, sizeof(buffer), tag.c_str());
					if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
					{
						tag = std::string(buffer);
					}
				});
			
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Components"))
		{
			ImGui::OpenPopup("AddComponents");
		}
		if (ImGui::BeginPopup("AddComponents"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectedEntity.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Circle Renderer"))
			{
				m_SelectedEntity.AddComponent<CircleRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Rigid Body"))
			{
				m_SelectedEntity.AddComponent<RigidBody2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Box Collider"))
			{
				m_SelectedEntity.AddComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Circle Collider"))
			{
				m_SelectedEntity.AddComponent<CircleCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		if (entity.HasComponent<TransformComponent>())
		{
			DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
				{
					DrawVec3Control("Position", component.m_Position);

					glm::vec3 rotationDegree = glm::degrees(component.m_Rotation);
					DrawVec3Control("Rotation", rotationDegree);
					component.m_Rotation = glm::radians(rotationDegree);

					DrawVec3Control("Scale", component.m_Scale, 1.f);
				});

		}

		if (entity.HasComponent<CameraComponent>())
		{
			
			DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
				{
					auto& camera = component.m_Camera;

					const char* CameraProjectionTypeString[(int)SceneCamera::ProjectionType::Max] = {
						"Orthographic",
						"Perspective",
					};
					const char* CurrentTypeString = CameraProjectionTypeString[(int)camera.GetProjectionType()];

					ImGui::Checkbox("Main Camera", &component.m_bIsMainCamera);

					if (ImGui::BeginCombo("Projection Type", CurrentTypeString))
					{
						for (int i = 0; i < (int)SceneCamera::ProjectionType::Max; ++i)
						{
							bool bIsSelected = CurrentTypeString == CameraProjectionTypeString[i];
							if (ImGui::Selectable(CameraProjectionTypeString[i], bIsSelected))
							{
								camera.SetProjectionType((SceneCamera::ProjectionType)i);
								CurrentTypeString = CameraProjectionTypeString[i];
							}

							if (bIsSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					ImGui::Separator();

					switch (camera.GetProjectionType())
					{
					case SceneCamera::ProjectionType::Orthographic:
					{
						float fSize = camera.GetOrthographicSize();
						if (ImGui::DragFloat("Size", &fSize, 0.1f, 1.f, 10.f))
						{
							camera.SetOrthographicSize(fSize);
						}

						float fNear = camera.GetOrthographicNearClip();
						if (ImGui::DragFloat("Near Clip", &fNear, 0.1f, -10.f, 10.f))
						{
							camera.SetOrthographicNearClip(fNear);
						}

						float fFar = camera.GetOrthographicFarClip();
						if (ImGui::DragFloat("Far Clip", &fFar, 0.1f, 10.f, 100.f))
						{
							camera.SetOrthographicFarClip(fFar);
						}
						break;
					}
					case SceneCamera::ProjectionType::Perspective:
					{
						float fFov = camera.GetPerspectiveVerticalFOV();
						if (ImGui::DragFloat("Vertical Fov", &fFov, 0.1f, 0.f, 135.f))
						{
							camera.SetPerspectiveVerticalFOV(fFov);
						}

						float fNear = camera.GetPerspectiveNearClip();
						if (ImGui::DragFloat("Near Clip", &fNear, 0.1f, 0.1f, 1000.f))
						{
							camera.SetPerspectiveNearClip(fNear);
						}

						float fFar = camera.GetPerspectiveFarClip();
						if (ImGui::DragFloat("Far Clip", &fFar, 0.1f, 100.f, 10000.f))
						{
							camera.SetPerspectiveFarClip(fFar);
						}
						break;
					}
					default:
						break;
					}
				}, true);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
				{
					auto& color = component.m_Color;
					ImGui::ColorEdit4("Color", glm::value_ptr(color));

					if (ImGui::Button("Texture", ImVec2(200.f, 0.f)))
					{
						component.m_Texture = {};
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ASSERT_PATH"))
						{
							auto data = (const char*)payload->Data;
							auto path = std::filesystem::path(data);
							auto fileExtension = path.filename().extension();
							if (fileExtension == ".png")
								component.m_Texture = DAZEL::Texture2D::Create(path.string());
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::DragInt("TileFactor", &component.m_nTileFactor, 1.f, 1.f, 100.f);
				}, true);
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](CircleRendererComponent& component)
				{
					auto& color = component.m_Color;
					ImGui::ColorEdit4("Color", glm::value_ptr(color));

					ImGui::DragFloat("Thickness", &component.m_fThickness, 0.01f, 0.01f, 0.5f);
					ImGui::DragFloat("Fade", &component.m_fFade, 0.001f, 0.001f, 0.1f);
				}, true);
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			DrawComponent<RigidBody2DComponent>("Rigid Body", entity, [](RigidBody2DComponent& component)
				{

					const char* BodyTypeString[(int)BodyType::MAX] = {
						"Static",
						"Dynamic",
						"Kinematic"
					};
					const char* CurrentTypeString = BodyTypeString[(int)component.m_Type];

					if (ImGui::BeginCombo("Body Type", CurrentTypeString))
					{
						for (int i = 0; i < (int)SceneCamera::ProjectionType::Max; ++i)
						{
							bool bIsSelected = CurrentTypeString == BodyTypeString[i];
							if (ImGui::Selectable(BodyTypeString[i], bIsSelected))
							{
								component.m_Type = (BodyType)i;
								CurrentTypeString = BodyTypeString[i];
							}

							if (bIsSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::Checkbox("Fixed Rotation", &component.m_bFixedRotation);
				}, true);
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			DrawComponent<BoxCollider2DComponent>("Box Collider", entity, [](BoxCollider2DComponent& component)
				{
					DrawVec2Control("Size", component.m_Size);
					DrawVec2Control("Offset", component.m_Offset);
					
					ImGui::DragFloat("Density", &component.m_fDensity, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Friction", &component.m_fFriction, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Restitution", &component.m_fRestitution, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Restitution Threshold", &component.m_fRestitutionThreshold, 0.01f, 0.f, 10.f);
				}, true);
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			DrawComponent<CircleCollider2DComponent>("Circle Collider", entity, [](CircleCollider2DComponent& component)
				{
					DrawVec2Control("Offset", component.m_Offset);
					ImGui::DragFloat("Radius", &component.m_fRadius, 0.1f, 0.01f, 0.5f);

					ImGui::DragFloat("Density", &component.m_fDensity, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Friction", &component.m_fFriction, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Restitution", &component.m_fRestitution, 0.01f, 0.f, 1.f);
					ImGui::DragFloat("Restitution Threshold", &component.m_fRestitutionThreshold, 0.01f, 0.f, 10.f);
				}, true);
		}
	}
}
