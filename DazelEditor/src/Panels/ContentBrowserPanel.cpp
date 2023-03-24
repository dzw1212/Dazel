#include "DazelPCH.h"
#include "ContentBrowserPanel.h"

#include "Dazel.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"

namespace DAZEL
{
	constexpr const char* s_ContentBrowserInitPath = "./assert/";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDir(s_ContentBrowserInitPath)
	{

	}
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy Panel:");
		ImGui::Separator();

		for (const auto& directory : m_CurrentDir)
		{
			if (ImGui::Button(directory.filename().string().c_str()))
			{

			}
		}

		ImGui::End();
	}
}
