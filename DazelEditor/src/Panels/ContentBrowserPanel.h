#pragma once

#include <filesystem>

namespace DAZEL
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDir;
	};
}
