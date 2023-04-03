#include "EditorLayer.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Odd/Scene/SceneSerializer.h"
#include "Odd/Utils/PlatformUtils.h"
#include "ImGuizmo.h"
#include "Odd/Math/Math.h"

namespace Odd
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
{

}

void EditorLayer::OnAttach()
{
	ODD_PROFILE_FUNCTION();
	m_SpriteSheet = Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/planes.png");

	m_RedPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet,   { 1, 2 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });
	m_YellowPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet,{ 0, 3 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });
	m_GreenPlane = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 3 }, { 88.0f, 73.5f }, { 1.0f, 1.0f });

	m_CameraController.SetZoomLevel(5.0f);

    FramebufferSpecification spec;
    spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
    spec.Width = 1280.0f;
    spec.Height = 720.0f;
    m_FrameBuffer = FrameBuffer::Create(spec);

    m_ActiveScene = CreateRef<Scene>();

    m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    
    //// Open This Scene as the Default One.
    //SceneSerializer serializer(m_ActiveScene);
    //m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    //serializer.Deserialize("D:/OddStoneGames/Odd/Odd-Editor/src/Assets/Scenes/3DCube.odd");

}

void EditorLayer::OnDetach()
{
	ODD_PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(Timestep timestep)
{
	// Update
	ODD_PROFILE_FUNCTION();

    // Resize
    if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
        m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    if (m_ViewportFocused)
        m_CameraController.OnUpdate(timestep);
        
    m_EditorCamera.OnUpdate(timestep);

	// Render
	Renderer2D::ResetStats();
    m_FrameBuffer->Bind();
	RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();

    // Clear our entity ID attachment to -1
    m_FrameBuffer->ClearColorAttachment(1, -1);

    // Update Scene Editor
    m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);

    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_ViewportBounds[0].x;
    my -= m_ViewportBounds[0].y;
    glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
    my = viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    {
        int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
        m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
    }

	//Renderer2D::DrawQuad({ -1.5f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_RedPlane);
	//Renderer2D::DrawQuad({  0.0f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_YellowPlane);
	//Renderer2D::DrawQuad({  1.5f, 0.0f, 0.3f }, { 1.0f, 0.829f }, m_GreenPlane);
	
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
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 150.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
                NewScene();

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
                OpenScene();

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                SaveSceneAs();

            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) Application::Get().Close();

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    m_SceneHierarchyPanel.OnImGuiRender();
    m_ContentBrowserPanel.OnImGuiRender();

    Renderer2D::Statistics stats = Renderer2D::GetStats();

    ImGui::Begin("Stats:");
    std::string name = m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag : "None";
    ImGui::Text("Hovered Entity: %s", name.c_str());
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("Renderer 2D Stats:");
    ImGui::Separator();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
    ImGui::Begin("Viewport");

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    // Gizmos
    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

    // Editor Camera
    const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
    glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

    // Setup ImGuizmo
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

    // Draw Grid
    //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
    if (selectedEntity && m_GizmoType != -1)
    {

        // Camera
        // Runtime Camera that we get from entity.
        // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        // const glm::mat4& cameraProjection = camera.GetProjection();
        // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        // Entity Transform
        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        // Snapping
        bool snap = Input::IsKeyPressed(Key::LeftControl);
        float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 10.f : 0.1f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), 
            nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Math::DecomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }

    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

    void EditorLayer::OnEvent(Event& event)
    {
	    m_CameraController.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)  return false;
    
        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        bool gizmoInUse = ImGuizmo::IsUsing();

        switch (e.GetKeyCode())
        {
            // File Shortcuts
            case Key::N: if (control) NewScene(); break;
            case Key::O: if (control) OpenScene(); break;
            case Key::S: if (control && shift) SaveSceneAs(); break;

            // Gizmos
            case Key::Q: m_GizmoType = -1; break;
            case Key::W: if(!gizmoInUse) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; break;
            case Key::E: if(!gizmoInUse) m_GizmoType = ImGuizmo::OPERATION::ROTATE; break;
            case Key::R: if(!gizmoInUse) m_GizmoType = ImGuizmo::OPERATION::SCALE; break;

            default: break;
        }

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mouse::ButtonLeft && m_ViewportHovered  && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
            // Mouse Picking!
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        std::string filePath = FileDialogs::OpenFile("Odd Scene (*.odd)\0*.odd\0");
        if (!filePath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filePath);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filePath = FileDialogs::SaveFile("Odd Scene(*.odd)\0*.odd\0");
        if (!filePath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filePath);
        }
    }

}