#include "ContentBrowserPanel.h"
#include <ImGui/include/imgui.h>

namespace Odd
{
	// To be changed when we have multiple projects!
	const std::filesystem::path s_AssetsPath = "D:/OddStoneGames/Odd/Odd-Editor/src/Assets";

	ContentBrowserPanel::ContentBrowserPanel() 
		: m_CurrentDirectory(s_AssetsPath) 
	{
		m_DirectoryIcon = Texture2D::Create("D:/OddStoneGames/Odd/Odd-Editor/src/Assets/Icons/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("D:/OddStoneGames/Odd/Odd-Editor/src/Assets/Icons/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != s_AssetsPath)
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
			
			auto relativePath = std::filesystem::relative(path, s_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}