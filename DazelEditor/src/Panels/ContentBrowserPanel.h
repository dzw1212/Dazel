#pragma once

#include <filesystem>
#include "Dazel/Renderer/Texture.h"

namespace DAZEL
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_PNGIcon;
		Ref<Texture2D> m_ShaderIcon;
		Ref<Texture2D> m_SceneIcon;

	};
}
