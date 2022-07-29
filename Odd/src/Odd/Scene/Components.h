#pragma once

#include "Odd/Scene/SceneCamera.h"
#include "ScriptableEntity.h"
#include "glm.hpp"

namespace Odd
{
	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent& other) : Tag(other.Tag) {}
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) : Transform(other.Transform) {}
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) : Color(other.Color) {}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;
		
		void(* OnCreateFunction)(ScriptableEntity*) = nullptr;
		void(* OnDestroyFunction)(ScriptableEntity*) = nullptr;
		void(* OnUpdateFunction)(ScriptableEntity*, Timestep) = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };
			
			OnCreateFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { static_cast<T*>(instance)->OnUpdate(ts); };
		}
	};
}