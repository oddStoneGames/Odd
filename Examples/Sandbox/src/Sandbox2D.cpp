#include "Sandbox2D.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Odd::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Odd::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Odd::RenderCommand::Clear();

	Odd::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Odd::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	
	Odd::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", &m_SquareColor[0]);
	ImGui::End();
}

void Sandbox2D::OnEvent(Odd::Event& event)
{
	m_CameraController.OnEvent(event);
}