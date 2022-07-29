#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Odd.h"

namespace Odd
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Scene> m_ActiveScene;

		Ref<Texture2D>	m_SpriteSheet;
		Ref<SubTexture2D> m_RedPlane, m_YellowPlane, m_GreenPlane;
		Ref<FrameBuffer> m_FrameBuffer;

		OrthographicCameraController m_CameraController;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}