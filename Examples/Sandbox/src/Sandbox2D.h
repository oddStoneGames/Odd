#pragma once

#include "Odd.h"

class Sandbox2D : public Odd::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Odd::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Odd::Event& event) override;

private:
	Odd::Ref<Odd::Shader>       m_SquareShader;
	Odd::Ref<Odd::VertexArray>  m_SquareVAO;
	Odd::Ref<Odd::Texture>		m_SquareTexture;
	Odd::Ref<Odd::Texture>		m_Square5Texture;
	Odd::Ref<Odd::Texture2D>	m_SpriteSheet;
	Odd::Ref<Odd::SubTexture2D>	m_RedPlane, m_YellowPlane, m_GreenPlane;

	glm::vec4 m_Square1Color = glm::vec4(0.873f, 1.000f, 0.284f, 1.000f);
	glm::vec4 m_Square3Color = glm::vec4(0.000f, 0.515f, 0.515f, 1.000f);
	glm::vec4 m_Square4Color = glm::vec4(0.239f, 0.463f, 0.220f, 0.400f);
	float m_Square4TilingFactor = 2.0f;
	float m_Square5TilingFactor = 2.0f;
	
	float m_Square5Depth = -0.1f;
	float m_Square5Rotation = 45.0f;
	Odd::OrthographicCameraController m_CameraController;
};