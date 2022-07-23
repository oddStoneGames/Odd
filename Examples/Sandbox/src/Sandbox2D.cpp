#include "Sandbox2D.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	ODD_PROFILE_FUNCTION();
	m_SquareTexture = Odd::Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Logo.jpg");
	m_Square5Texture = Odd::Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Background.png");
}

void Sandbox2D::OnDetach()
{
	ODD_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Odd::Timestep timestep)
{
	// Update
	ODD_PROFILE_FUNCTION();
	
	m_CameraController.OnUpdate(timestep);

	// Render
	Odd::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Odd::RenderCommand::Clear();

	{
		ODD_PROFILE_SCOPE("Renderer Draw");
		Odd::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Odd::Renderer2D::DrawQuad({ -0.5f,  0.5f }, { 1.0f, 1.0f }, m_Square1Color);
		Odd::Renderer2D::DrawQuad({ 0.5f,  0.5f }, { 1.0f, 1.0f }, m_SquareTexture);
		Odd::Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 1.0f, 1.0f }, m_Square3Color, m_SquareTexture);
		Odd::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.0f, 1.0f }, m_Square4Color, m_SquareTexture, m_Square4TilingFactor);
		Odd::Renderer2D::DrawRotatedQuad({ 0.0f,  0.0f, m_Square5Depth }, m_Square5Rotation, { 5.0f, 5.0f }, m_Square5Texture, m_Square5TilingFactor);

		Odd::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ODD_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("1st Square Color",  &m_Square1Color[0]);
	ImGui::ColorEdit4("3rd Square Color",  &m_Square3Color[0]);
	ImGui::ColorEdit4("4th Square Color", &m_Square4Color[0]);
	ImGui::SliderFloat("4th Square Texture Size", &m_Square4TilingFactor, 0.0f, 10.0f);
	ImGui::SliderFloat("5th Square Texture Size", &m_Square5TilingFactor, 0.0f, 10.0f);
	ImGui::SliderFloat("5th Square Depth", &m_Square5Depth, -1.0f, 1.0f);
	ImGui::SliderFloat("5th Square Rotation", &m_Square5Rotation, -360.0f, 360.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Odd::Event& event)
{
	m_CameraController.OnEvent(event);
}