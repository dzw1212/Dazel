#include "DazelPCH.h"
#include "ContentBrowserPanel.h"

#include "Dazel.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"

namespace DAZEL
{
	constexpr const char* s_ContentBrowserInitPath = "assert";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory(s_ContentBrowserInitPath), m_CurrentDirectory(s_ContentBrowserInitPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resource/Icon/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resource/Icon/ContentBrowser/FileIcon.png");
		m_PNGIcon = Texture2D::Create("Resource/Icon/ContentBrowser/PNGIcon.png");
		m_ShaderIcon = Texture2D::Create("Resource/Icon/ContentBrowser/ShaderIcon.png");
		m_SceneIcon = Texture2D::Create("Resource/Icon/ContentBrowser/SceneIcon.png");
	}
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser Panel:");
		ImGui::Separator();

		if (m_CurrentDirectory != m_BaseDirectory)
		{
			if (ImGui::Button("<-"))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			ImGui::Separator();
		}

		static float fPaddingSize = 10.f;
		static float fThumbnailSize = 100.f;
		float fCellSize = fThumbnailSize + fPaddingSize;

		float fPanelWidth = ImGui::GetContentRegionAvail().x;
		int nColumnCount = (int)(fPanelWidth / fCellSize);
		if (nColumnCount < 1)
			nColumnCount = 1;

		ImGui::Columns(nColumnCount, 0, false);


		for (const auto& child : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			auto fullPath = child.path();
			auto relativePath = std::filesystem::relative(child, s_ContentBrowserInitPath);
			auto filename = relativePath.filename();
			auto fileExtension = child.path().filename().extension();

			Ref<Texture2D> icon;
			if (child.is_directory())
				icon = m_DirectoryIcon;
			else
			{	
				if (fileExtension == ".png")
					icon = m_PNGIcon;
				else if (fileExtension == ".vert" || fileExtension == ".frag")
					icon = m_ShaderIcon;
				else if (fileExtension == ".dazel")
					icon = m_SceneIcon;
				else
					icon = m_FileIcon;
			}
			ImGui::PushID(filename.string().c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, { 15.f / 255.f, 15.f / 255.f, 15.f / 255.f, 1.f });
			ImGui::ImageButton((ImTextureID)icon->GetId(), { fThumbnailSize, fThumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();
			

			if (!child.is_directory())
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ASSERT_PATH", fullPath.string().c_str(), fullPath.string().size() + 1);
					ImGui::EndDragDropSource();
				}
			}


			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (child.is_directory())
				{
					m_CurrentDirectory /= child.path().filename();
				}
			}
			ImGui::TextWrapped(filename.string().c_str());
				
			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);
		ImGui::End();
	}
}
