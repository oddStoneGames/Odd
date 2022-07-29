#pragma once

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

		Entity m_SquareEntity;

		Entity m_PrimaryCamera, m_SecondaryCamera;
		bool m_IsPrimaryCameraPrimary = true;

		glm::vec2 primaryCameraPosition{ 0.0f, 0.0f };
		float	  primaryCameraRotation = 0.0f;
		float	  primaryCameraSize = 10.0f;

		glm::vec2 secondaryCameraPosition{ 0.0f, 0.0f };
		float	  secondaryCameraRotation = 0.0f;
		float	  secondaryCameraSize = 100.0f;

		Ref<Scene> m_ActiveScene;

		Ref<Texture2D>	m_SpriteSheet;
		Ref<SubTexture2D> m_RedPlane, m_YellowPlane, m_GreenPlane;
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 squarePosition{ 0.0f, -4.41f };
		float squareRotation = 0.0f;
		glm::vec2 squareSize{14.0f, 8.0f};

		OrthographicCameraController m_CameraController;
	};
}