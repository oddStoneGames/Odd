#include "Sandbox2D.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareTexture = Odd::Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Logo.jpg");
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
	
	Odd::Renderer2D::DrawQuad({ -0.75f,  0.75f }, { 1.0f, 1.0f }, m_SquareColor1);
	Odd::Renderer2D::DrawQuad({  0.75f,  0.75f }, { 1.0f, 1.0f }, m_SquareTexture);
	Odd::Renderer2D::DrawQuad({ -0.75f, -0.75f }, { 1.0f, 1.0f }, m_SquareColor3, m_SquareTexture);
	Odd::Renderer2D::DrawQuad({  0.75f, -0.75f, -0.1f }, { 1.0f, 1.0f }, glm::vec4(1.0f));
	Odd::Renderer2D::DrawQuad({  0.75f, -0.75f }, { 1.0f, 1.0f }, m_SquareColor4, m_SquareTexture, glm::vec2(2.0f));
	
	Odd::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("First Square Color",  &m_SquareColor1[0]);
	ImGui::ColorEdit4("Third Square Color",  &m_SquareColor3[0]);
	ImGui::ColorEdit4("Fourth Square Color", &m_SquareColor4[0]);
	ImGui::End();
}

void Sandbox2D::OnEvent(Odd::Event& event)
{
	m_CameraController.OnEvent(event);
}