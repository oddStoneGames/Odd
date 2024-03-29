#pragma once

#include "Odd/Scene/SceneCamera.h"
#include "Odd/Renderer/Texture.h"
#include "Odd/Renderer/SubTexture2D.h"
#include "Odd/Core/UUID.h"
#include "Odd/Core/Log.h"
#include "Odd/Audio/AudioSource.h"
#include "Odd/Audio/AudioListener.h"

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/gtx/quaternion.hpp"
#include <filesystem>

#include "box2d/b2_body.h"

namespace Odd
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent& other) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other) : Tag(other.Tag) {}
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> Texture;
		
		// SubTexture Data
		Ref<SubTexture2D> Subtexture;
		glm::vec2 SubtextureCoords = {0.0f, 0.0f};
		glm::vec2 SubtextureCellSize = { 0.0f, 0.0f };
		glm::vec2 SubtextureSpriteSize = { 1.0f, 1.0f };

		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) 
			: Color(other.Color), TilingFactor(other.TilingFactor), 
			SubtextureCoords(other.SubtextureCoords), SubtextureCellSize(other.SubtextureCellSize), 
			SubtextureSpriteSize(other.SubtextureSpriteSize)
		{
			if (other.Texture && std::filesystem::exists(other.Texture->GetPath()))
			{
				Texture = Texture2D::Create(other.Texture->GetPath());

				if (other.Subtexture)
				{
					// Copy & Create Subtexture.
					Subtexture = SubTexture2D::CreateFromCoords(Texture, SubtextureCoords, SubtextureCellSize, SubtextureSpriteSize);
				}
			}
		}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool fixedAspectRatio = false;
		glm::vec4 ClearColor{ 0.1f, 0.1f, 0.1f, 1.0f };

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	// Forward Declaration
	class ScriptableEntity;
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	// Physics 2D
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		// Returns this Rigidbody2D's Position.
		glm::vec2 GetPosition()
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return glm::vec2(0.0f);
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 pos = body->GetPosition();

			return { pos.x, pos.y };
		}

		// Returns this Rigidbody2D's Linear Velocity.
		glm::vec2 GetLinearVelocity()
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return glm::vec2(0.0f);
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 linearVelocity = body->GetLinearVelocity();

			return { linearVelocity.x, linearVelocity.y };
		}

		// Set this Rigidbody2D's Linear Velocity.
		void SetLinearVelocity(const glm::vec2& linearVelocity)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 b2linearVelocity = b2Vec2(linearVelocity.x, linearVelocity.y);
			body->SetLinearVelocity(b2linearVelocity);

			return;
		}

		// Returns this Rigidbody2D's Angular Velocity.
		float GetAngularVelocity()
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return 0.0f;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			return body->GetAngularVelocity();
		}

		// Set this Rigidbody2D's Angular Velocity.
		void SetAngulrVelocity(const float angularVelocity)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			body->SetAngularVelocity(angularVelocity);

			return;
		}

		void ApplyForce(const glm::vec2& force, const glm::vec2& point)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 b2force = b2Vec2(force.x, force.y);
			b2Vec2 b2point = b2Vec2(point.x, point.y);
			
			body->ApplyForce(b2force, b2point, true);
		}

		void ApplyForceToCenter(const glm::vec2& force)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 b2force = b2Vec2(force.x, force.y);

			body->ApplyForceToCenter(b2force, true);
		}

		void ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 b2impulse = b2Vec2(impulse.x, impulse.y);
			b2Vec2 b2point = b2Vec2(point.x, point.y);

			body->ApplyLinearImpulse(b2impulse, b2point, true);
		}

		void ApplyLinearImpulseToCenter(const glm::vec2& impulse)
		{
			if (RuntimeBody == nullptr)
			{
				DEBUG_CORE_ERROR("Runtime Body null.");
				return;
			}

			b2Body* body = (b2Body*)RuntimeBody;
			b2Vec2 b2impulse = b2Vec2(impulse.x, impulse.y);

			body->ApplyLinearImpulseToCenter(b2impulse, true);
		}

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct AudioSourceComponent
	{
		Ref<AudioSource> audioSource;

		bool PlayOnStart = true;
		bool Loop = false;
		float Gain = 1.0f;
		float Pitch = 1.0f;

		AudioSourceComponent() = default;
		AudioSourceComponent(const AudioSourceComponent& other) 
			: PlayOnStart(other.PlayOnStart), Loop(other.Loop), Gain(other.Gain), Pitch(other.Pitch)
		{
			if (other.audioSource)
			{
				audioSource = AudioSource::Create(other.audioSource->GetAudioFilePath());

				// Register Audio Source Attribute changes.
				audioSource->SetLoop(Loop);
				audioSource->SetGain(Gain);
				audioSource->SetPitch(Pitch);
			}
		}
		
		// Play the currently bound audio from this audio source.
		void Play()
		{
			if (audioSource)
			{
				// Register Audio Source Attribute changes.
				audioSource->SetLoop(Loop);
				audioSource->SetGain(Gain);
				audioSource->SetPitch(Pitch);

				audioSource->Play();
			}
		}

		// Pause the currently playing audio bound to this audio source.
		void Pause()
		{
			if (audioSource)
				audioSource->Pause();
		}

		// Stop the currently playing audio bound to this audio source.
		void Stop()
		{
			if (audioSource)
				audioSource->Stop();
		}

		// Register Audio Source Attribute changes.
		void PushAttributeChanges()
		{
			if (audioSource)
			{
				audioSource->SetLoop(Loop);
				audioSource->SetGain(Gain);
				audioSource->SetPitch(Pitch);
			}
		}
	};

	struct AudioListenerComponent
	{
		// If True, then this audio listener will have the same
		// positional attributes as the transform component.
		bool PositionSameAsEntity = true;
		
		// Master Gain
		float Gain = 1.0f;
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Velocity{ 0.0f, 0.0f, 0.0f };

		// Orientation
		glm::vec3 At{ 0.0f, 0.0f, -1.0f };
		glm::vec3 Up{ 0.0f, 1.0f,  0.0f };

		AudioListenerComponent() = default;
		AudioListenerComponent(const AudioListenerComponent& other)
			: PositionSameAsEntity(other.PositionSameAsEntity), Gain(other.Gain), Position(other.Position),
			Velocity(other.Velocity), At(other.At), Up(other.Up)
		{
			PushAttributeChanges(Position);
		}

		void PushAttributeChanges(const glm::vec3& position)
		{
			AudioListener::SetGain(Gain);
			AudioListener::SetPosition(position);
			AudioListener::SetVelocity(Velocity);
			AudioListener::SetOrientation(At, Up);
		}
	};
}