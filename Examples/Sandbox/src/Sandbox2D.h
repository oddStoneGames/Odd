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
	glm::vec4 m_SquareColor = glm::vec4(0.873f, 1.000f, 0.284f, 1.000f);

	Odd::OrthographicCameraController m_CameraController;
};