#pragma once

#include "Odd/Core/UUID.h"
#include "Odd/Scene/Components.h"
#include "Scene.h"

#include "entt.hpp"

namespace Odd
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}
		Entity(const Entity& other) = default;

		// Returns the Entity of given name in this scene.
		Entity GetEntityByName(const std::string& name)
		{
			return m_Scene->GetEntityByName(name);
		}

		// Returns the scene this entity belongs to.
		Scene* GetScene() const { return m_Scene; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
				DEBUG_CORE_ERROR("already has {0} component!", typeid(T).name());
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded(*this);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded(*this);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>())
				DEBUG_CORE_ERROR("Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>())
				DEBUG_CORE_ERROR("Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator entt::entity() const { return m_EntityHandle; }

		operator bool() const { return m_EntityHandle != entt::null; }
		
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const 
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}