#pragma once
#include <filesystem>
#include <string>
#include "Odd/Renderer/Texture.h"

namespace Odd
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::wstring string_to_wstring(const std::string& str);
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_FileIcon, m_DirectoryIcon;
	};
}