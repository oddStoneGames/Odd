#include "oddpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Odd/Renderer/Renderer2D.h"

namespace Odd
{
    Scene::Scene()
    {
        
    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };

        auto& tag = entity.AddComponent<TagComponent>(name);
        tag.Tag = name.empty() ? "Entity" : name;

        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        // Update Scripts
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                // TODO: Move to OnScenePlay
                if (!nsc.Instance)
                {
                    nsc.InstantiateFunction();
                    nsc.Instance->m_Entity = Entity{ entity, this };
                    nsc.OnCreateFunction(nsc.Instance);
                }

                nsc.OnUpdateFunction(nsc.Instance, ts);
            });
        }


        // Render Sprites
        SceneCamera* mainCamera = nullptr;
        glm::mat4* mainCameratransform = nullptr;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    //DEBUG_CORE_WARN("Projection Matrix: {0}", glm::to_string(mainCamera->GetProjection()));
                    mainCameratransform = &transform.Transform;
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *mainCameratransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform, spriteRenderer.Color);
            }
            Renderer2D::EndScene();
        }

    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize non-Fixed Aspect Ratio Cameras!
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

}