#pragma once
#include <filesystem>
#include "Odd/Renderer/Texture.h"

namespace Odd
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_FileIcon, m_DirectoryIcon;
	};
}