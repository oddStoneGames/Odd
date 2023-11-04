#include "ContentBrowserPanel.h"
#include <ImGui/include/imgui.h>
#include <locale>
#include <codecvt>

namespace Odd
{
	// To be changed when we have multiple projects!
	extern const std::filesystem::path g_AssetsPath = PROJECT_DIR"SandboxProject";

	ContentBrowserPanel::ContentBrowserPanel() 
		: m_CurrentDirectory(g_AssetsPath) 
	{
		m_DirectoryIcon = Texture2D::Create(PROJECT_DIR"src/Assets/Icons/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create(PROJECT_DIR"src/Assets/Icons/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != g_AssetsPath)
		{
			if (ImGui::Button("<-"))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			
			auto relativePath = std::filesystem::relative(path, g_AssetsPath);
			std::string filenameString = relativePath.filename().string();
			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				std::wstring wideString = string_to_wstring(relativePath.string());
				const wchar_t* itemPath = wideString.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

    std::wstring ContentBrowserPanel::string_to_wstring(const std::string &str)
    {
        std::wstring wstr(str.size(), L' '); // Overestimate number of code points.
    	wstr.resize(mbstowcs(&wstr[0], str.c_str(), str.size())); // Shrink to fit.
    	return wstr;
    }
}