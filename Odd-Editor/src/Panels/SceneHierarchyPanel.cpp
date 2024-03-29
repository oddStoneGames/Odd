#include "SceneHierarchyPanel.h"
#include "Odd/Renderer/Texture.h"
#include "Odd/Renderer/SubTexture2D.h"

#include <ImGui/include/imgui.h>
#include <ImGui/include/imgui_internal.h>

#include "glm/gtc/type_ptr.hpp"

#include <filesystem>

namespace Odd
{
	extern const std::filesystem::path g_AssetsPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}
	
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context)
		{
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID, m_Context.get() };
					DrawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create New Entity"))
					m_Context->CreateEntity("New Entity");

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		// Right-click on item
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
			m_Context->DestroyEntity(entity);
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)(typeid(T).hash_code()), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button(("+"), ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{				
				uifunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy(buffer, tag.c_str(), sizeof(buffer) - 1);  // Leave space for the null terminator
			buffer[sizeof(buffer) - 1] = '\0'; // Explicitly null-terminate
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<NativeScriptComponent>("Native Script");
			DisplayAddComponentEntry<AudioSourceComponent>("Audio Source");
			DisplayAddComponentEntry<AudioListenerComponent>("Audio Listener");
			
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Position", component.Translation);

			glm::vec3 degreesRotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", degreesRotation);
			component.Rotation = glm::radians(degreesRotation);

			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);
			ImGui::ColorEdit4("Clear Color", &component.ClearColor[0]);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float persFOV = camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &persFOV, 0.1f), 0.0f, 89.0f)
					camera.SetPerspectiveFOV(persFOV);

				float persNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near Clip", &persNear, 0.1f), 0.001f, 1000.0f)
					camera.SetPerspectiveNearClip(persNear);

				float persFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far Clip", &persFar, 0.1f), 0.001f, 1000.0f)
					camera.SetPerspectiveFarClip(persFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Ortho Size", &orthoSize, 0.1f), 0.001f, 1000.0f)
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f), 0.001f, 1000.0f)
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f), 0.001f, 1000.0f)
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::Text("Texture");
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f - 30.0f);
			auto textureID = component.Texture ? component.Texture->GetRendererID() : 0;
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(60.0f, 60.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetsPath) / path;
					std::string texturePathString = texturePath.string();
					component.Texture = Texture2D::Create(texturePathString);
				}

				ImGui::EndDragDropTarget();
			}
			if (component.Texture)
			{
				ImGui::NewLine();
				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - 45.0f);

				// Load a bigger font
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
				ImGui::Text("Subtexture");
				ImGui::PopFont();

				// Only show subtexture data when there is an existing texture.
				ImGui::DragFloat2("Coordinates", &component.SubtextureCoords[0], 0.01f, 0.0f);
				ImGui::DragFloat2("Cell Size", &component.SubtextureCellSize[0], 0.01f, 0.0f);
				ImGui::DragFloat2("Sprite Size", &component.SubtextureSpriteSize[0], 0.01f, 0.0f);

				if (component.SubtextureCellSize.x > 0.0f && component.SubtextureCellSize.y > 0.0f &&
					component.SubtextureSpriteSize.x > 0.0f && component.SubtextureSpriteSize.y > 0.0f)
				{
					// Create Subtexture only if the values are valid.
					component.Subtexture = SubTexture2D::CreateFromCoords(component.Texture, component.SubtextureCoords, component.SubtextureCellSize, component.SubtextureSpriteSize);
				}
				ImGui::NewLine();
			}
			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 1000.0f);
			ImGui::ColorEdit4("Color", &component.Color[0]);
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
		{
			const char* rigidbody2DTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentRigidbody2DTypeString = rigidbody2DTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentRigidbody2DTypeString))
			{
				for (int i = 0; i < 3; i++)
				{
					bool isSelected = currentRigidbody2DTypeString == rigidbody2DTypeStrings[i];
					if (ImGui::Selectable(rigidbody2DTypeStrings[i], isSelected))
					{
						currentRigidbody2DTypeString = rigidbody2DTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", &component.Offset[0]);
			ImGui::DragFloat2("Size", &component.Size[0]);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<AudioSourceComponent>("Audio Source", entity, [](auto& component)
		{
			ImGui::Text("Audio");
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f - 150.0f);
			
			std::string audioFileName = "None";
			if (component.audioSource)
			{
				std::filesystem::path audioFilePath = component.audioSource->GetAudioFilePath();
				audioFileName = audioFilePath.filename().string();
			}
			
			ImGui::InputText("##AudioFilePath", audioFileName.data(), sizeof(audioFileName), ImGuiInputTextFlags_ReadOnly);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path audioFilePath = std::filesystem::path(g_AssetsPath) / path;
					std::string audioFilePathString = audioFilePath.string();
					component.audioSource = AudioSource::Create(audioFilePathString);
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::NewLine();
			ImGui::Checkbox("Play On Start", &component.PlayOnStart);
			ImGui::Checkbox("Loop", &component.Loop);
			ImGui::DragFloat("Gain", &component.Gain, 0.01f, 0.0f);
			ImGui::DragFloat("Pitch", &component.Pitch, 0.01f, 0.0f);

			// Push the changes.
			component.PushAttributeChanges();
		});

		DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](auto& component)
		{
			ImGui::Checkbox("Position Same as Transform: ", &component.PositionSameAsEntity);
			if (!component.PositionSameAsEntity)
				ImGui::DragFloat3("Position", &component.Position[0], 0.01f);
			ImGui::DragFloat3("Velocity", &component.Velocity[0], 0.01f);

			ImGui::NewLine();
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - 45.0f);

			// Load a bigger font
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
			ImGui::Text("Orientation");
			ImGui::PopFont();

			ImGui::DragFloat3("At", &component.At[0], 0.01f);
			ImGui::DragFloat3("Up", &component.Up[0], 0.01f);
		});

		DrawComponent<NativeScriptComponent>("Native Script", entity, [](auto& component)
		{

		});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName, bool allowMultipleComponents)
	{
		if (!m_SelectionContext.HasComponent<T>() || allowMultipleComponents)
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}