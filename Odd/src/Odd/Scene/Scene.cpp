#include "oddpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "ScriptableEntity.h"
#include "Odd/Renderer/Renderer2D.h"

// TODO: Temporary architecture, change later.
#include "../../../../Odd-Editor/SandboxProject/Scripts/PlayerController.h"
#include "../../../../Odd-Editor/SandboxProject/Scripts/FollowPlayer.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Odd
{
    static b2BodyType Rigidbody2DTypeToBox2D(Rigidbody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
            case Odd::Rigidbody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
            case Odd::Rigidbody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
            case Odd::Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
        }

        DEBUG_CORE_ERROR("Unknown body type.");
        return b2_staticBody;
    }

    Scene::Scene()
    {
        
    }

    Scene::~Scene()
    {

    }

    template<typename Component>
    static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        auto view = src.view<Component>();
        for (auto e : view)
        {
            UUID uuid = src.get<IDComponent>(e).ID;
            entt::entity dstEnttID = enttMap.at(uuid);

            auto& component = src.get<Component>(e);
            dst.emplace_or_replace<Component>(dstEnttID, component);
        }
    }

    template<typename Component>
    static void CopyComponentIfExists(Entity dst, Entity src)
    {
        if (src.HasComponent<Component>())
            dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
    }

    Ref<Scene> Scene::Copy(Ref<Scene> other)
    {
        Ref<Scene> newScene = CreateRef<Scene>();

        newScene->m_ViewportWidth = other->m_ViewportWidth;
        newScene->m_ViewportHeight = other->m_ViewportHeight;

        auto& srcSceneRegistry = other->m_Registry;
        auto& dstSceneRegistry = newScene->m_Registry;
        std::unordered_map<UUID, entt::entity> enttMap;

        // Create entities in new scene.
        auto idView = srcSceneRegistry.view<IDComponent>();
        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
            const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
            enttMap[uuid] = newScene->CreateEntityWithUUID(uuid, name);
        }

        // Copy Components (Except IDComponent & TagComponent)
        CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<AudioSourceComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<AudioListenerComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

        return newScene;
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };

        entity.AddComponent<IDComponent>(uuid);
        auto& tag = entity.AddComponent<TagComponent>(name);
        tag.Tag = name.empty() ? "Entity" : name;

        entity.AddComponent<TransformComponent>();
        return entity;
    }

    Entity Scene::GetEntityByName(const std::string& name)
    {
        auto view = m_Registry.view<TagComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };

            if (entity.GetName() == name)
                return entity;
        }

        return Entity();
    }

    void Scene::DestroyEntity(Entity entity)
    {
        DEBUG_CORE_WARN("Destroying {0}!", entity.GetComponent<TagComponent>().Tag.c_str());
        m_Registry.destroy(entity);
    }

    void Scene::OnRuntimeStart()
    {
        // Initialize Physics2D World
        m_Physics2DWorld = new b2World({ 0.0f, -9.8f });

        auto view = m_Registry.view <Rigidbody2DComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2D(rb2d.Type);
            bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;

            b2Body* body = m_Physics2DWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rb2d.FixedRotation);
            rb2d.RuntimeBody = body;

            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

                b2PolygonShape boxShape;
                boxShape.SetAsBox(transform.Scale.x * bc2d.Size.x, transform.Scale.y * bc2d.Size.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bc2d.Density;
                fixtureDef.friction = bc2d.Friction;
                fixtureDef.restitution = bc2d.Restitution;
                fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

                b2Fixture* fixture = body->CreateFixture(&fixtureDef);
                bc2d.RuntimeFixture = fixture;
            }
        }

        // Create Scripts
        {
            ODD_PROFILE_SCOPE("NativeScripts::OnCreate()");
            m_Registry.view<NativeScriptComponent>().each([=](auto e, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    Entity entity = { e, this };
                    auto& tagComponent = entity.GetComponent<TagComponent>();

                    // TODO: Temporary, change later.
                    if (tagComponent.Tag.compare("Player") == 0)
                    {
                        nsc.Bind<PlayerController>();
                    }
                    else if (tagComponent.Tag.compare("Main Camera") == 0)
                    {
                        nsc.Bind<FollowPlayer>();
                    }
                    
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = entity;
                    nsc.Instance->OnCreate();
                }
            });
        }

        // Set the attributes of the audio listener because there is no other authority managing it.
        auto transformAudioListenerView = m_Registry.view<TransformComponent, AudioListenerComponent>();
        for (auto entity : transformAudioListenerView)
        {
            auto [transform, audioListener] = transformAudioListenerView.get<TransformComponent, AudioListenerComponent>(entity);

            // Set the Audio Listener Position to the transform's position.
            audioListener.PushAttributeChanges(audioListener.PositionSameAsEntity ? transform.Translation : audioListener.Position);
        }

        // Play the Audio Sources that are set to play on start!
        auto transformAudioView = m_Registry.view<TransformComponent, AudioSourceComponent>();
        for (auto entity : transformAudioView)
        {
            auto [transform, audioSource] = transformAudioView.get<TransformComponent, AudioSourceComponent>(entity);

            // Set the Audio Source Position to the transform's position.
            if (audioSource.AudioSource)
            {
                audioSource.AudioSource->SetPosition(transform.Translation);

                // Play on start.
                if (audioSource.PlayOnStart)
                    audioSource.Play();
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        delete m_Physics2DWorld;
        m_Physics2DWorld = nullptr;
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (uint32_t)entity);
        }
        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(Timestep ts)
    {
        // Update the attributes of the audio listener because there is no other authority managing it.
        auto transformAudioListenerView = m_Registry.view<TransformComponent, AudioListenerComponent>();
        for (auto entity : transformAudioListenerView)
        {
            auto [transform, audioListener] = transformAudioListenerView.get<TransformComponent, AudioListenerComponent>(entity);

            // Set the Audio Listener Position to the transform's position.
            audioListener.PushAttributeChanges(audioListener.PositionSameAsEntity ? transform.Translation : audioListener.Position);
        }

        // Update the position of the audio sources to be same as its transform's position.
        auto transformAudioView = m_Registry.view<TransformComponent, AudioSourceComponent>();
        for (auto entity : transformAudioView)
        {
            auto [transform, audioSource] = transformAudioView.get<TransformComponent, AudioSourceComponent>(entity);

            // Set the Audio Source Position to the transform's position.
            if (audioSource.AudioSource)
                audioSource.AudioSource->SetPosition(transform.Translation);
        }

        // Update Scripts
        {
            ODD_PROFILE_SCOPE("NativeScripts::OnUpdate()");
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                nsc.Instance->OnUpdate(ts);
            });
        }

        // Physics 2D
        {
            ODD_PROFILE_SCOPE("Physics2D");

            const int32_t velocityIterations = 6;
            const int32_t positionIterations = 2;

            m_Physics2DWorld->Step(ts, velocityIterations, positionIterations);

            // Retrieve transform from Box2D
            auto view = m_Registry.view<Rigidbody2DComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

                b2Body* body = (b2Body*)rb2d.RuntimeBody;
                const auto& position = body->GetPosition();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }

        // Render 2D
        SceneCamera* mainCamera = nullptr;
        glm::mat4 mainCameratransform { 0.0f };
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    mainCameratransform = transform.GetTransform();
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, mainCameratransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entity);
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

    void Scene::DuplicateEntity(Entity entity)
    {
        Entity newEntity = CreateEntity(entity.GetName());

        // Copy Components (Except IDComponent & TagComponent)
        CopyComponentIfExists<TransformComponent>(newEntity, entity);
        CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
        CopyComponentIfExists<CameraComponent>(newEntity, entity);
        CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
        CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
        CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
        CopyComponentIfExists<AudioSourceComponent>(newEntity, entity);
        CopyComponentIfExists<AudioListenerComponent>(newEntity, entity);
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
                return Entity(entity, this);
        }

        return {};
    }

    void Scene::OnComponentAdded(Entity entity)
    {
        if (entity.HasComponent<CameraComponent>())
        {
            entity.GetComponent<CameraComponent>().Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
        }
    }
}