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

		Ref<Shader>       m_SquareShader;
		Ref<VertexArray>  m_SquareVAO;
		Ref<Texture>		m_SquareTexture;
		Ref<Texture>		m_Square5Texture;
		Ref<Texture2D>	m_SpriteSheet;
		Ref<SubTexture2D>	m_RedPlane, m_YellowPlane, m_GreenPlane;
		Ref<FrameBuffer>  m_FrameBuffer;

		glm::vec4 m_Square1Color = glm::vec4(0.873f, 1.000f, 0.284f, 1.000f);
		glm::vec4 m_Square3Color = glm::vec4(0.000f, 0.515f, 0.515f, 1.000f);
		glm::vec4 m_Square4Color = glm::vec4(0.239f, 0.463f, 0.220f, 0.400f);
		float m_Square4TilingFactor = 2.0f;
		float m_Square5TilingFactor = 2.0f;

		float m_Square5Depth = -0.1f;
		float m_Square5Rotation = 45.0f;
		OrthographicCameraController m_CameraController;
	};
}