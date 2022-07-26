#include "EditorLayer.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Odd
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
{

}

void EditorLayer::OnAttach()
{
	ODD_PROFILE_FUNCTION();
	m_SquareTexture = Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Logo.jpg");
	m_Square5Texture = Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Background.png");
	m_SpriteSheet = Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/planes.png");

	m_RedPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet,   { 1, 2 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });
	m_YellowPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet,{ 0, 3 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });
	m_GreenPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 3 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });

	m_CameraController.SetZoomLevel(5.0f);

    FramebufferSpecification spec;
    spec.Width = 1280.0f;
    spec.Height = 720.0f;
    m_FrameBuffer = FrameBuffer::Create(spec);
}

void EditorLayer::OnDetach()
{
	ODD_PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(Timestep timestep)
{
	// Update
	ODD_PROFILE_FUNCTION();
	
    if(m_ViewportFocused)
	    m_CameraController.OnUpdate(timestep);

	// Render
	Renderer2D::ResetStats();
    m_FrameBuffer->Bind();
	RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();

	{
		ODD_PROFILE_SCOPE("Renderer Draw");
		Renderer2D::BeginScene(m_CameraController.GetCamera());

		Renderer2D::DrawQuad({ -0.5f,  0.5f }, { 1.0f, 1.0f }, m_Square1Color);
		Renderer2D::DrawQuad({ 0.5f,  0.5f }, { 1.0f, 1.0f }, m_SquareTexture);
		Renderer2D::DrawQuad({ -0.5f, -0.5f }, { 1.0f, 1.0f }, m_Square3Color, m_SquareTexture);
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.0f, 1.0f }, m_Square4Color, m_SquareTexture, m_Square4TilingFactor);
		Renderer2D::DrawRotatedQuad({ 0.0f,  0.0f, m_Square5Depth }, m_Square5Rotation, { 5.0f, 5.0f }, m_Square5Texture, m_Square5TilingFactor);
		
		for (float x = -10.0f; x < 10.0f; x += 1.0f)
		{
			for (float y = -10.0f; y < 10.0f; y += 1.0f)
			{
				glm::vec4 color = { (x + 10.0f) / 20.0f, 0.3f, (y + 10.0f) / 20.0f, 0.5f };
				Renderer2D::DrawQuad({ x, y, 0.1f }, { 0.95f, 0.95f }, color);
			}
		}
		
		Renderer2D::DrawQuad({ -1.5f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_RedPlane);
		Renderer2D::DrawQuad({  0.0f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_YellowPlane);
		Renderer2D::DrawQuad({  1.5f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_GreenPlane);

		Renderer2D::EndScene();
	}
    m_FrameBuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
	ODD_PROFILE_FUNCTION();

    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) Application::Get().Close();
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    Renderer2D::Statistics stats = Renderer2D::GetStats();
    ImGui::Begin("Renderer 2D Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
    ImGui::End();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("1st Square Color", &m_Square1Color[0]);
    ImGui::ColorEdit4("3rd Square Color", &m_Square3Color[0]);
    ImGui::ColorEdit4("4th Square Color", &m_Square4Color[0]);
    ImGui::SliderFloat("4th Square Texture Size", &m_Square4TilingFactor, 0.0f, 10.0f);
    ImGui::SliderFloat("5th Square Texture Size", &m_Square5TilingFactor, 0.0f, 10.0f);
    ImGui::SliderFloat("5th Square Depth", &m_Square5Depth, -1.0f, 1.0f);
    ImGui::SliderFloat("5th Square Rotation", &m_Square5Rotation, -360.0f, 360.0f);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
    {        
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

}