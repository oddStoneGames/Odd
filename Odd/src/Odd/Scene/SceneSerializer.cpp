#include "oddpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Odd
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w <<YAML::EndSeq;
		return out;
	}

	static std::string Rigidbody2DBodyTypeTostring(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static: return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic: return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		DEBUG_CORE_ERROR("Unknown body type.");
		return "Static";
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DBodyTypeFromString(std::string bodyType)
	{
		if (bodyType == "Static") return Rigidbody2DComponent::BodyType::Static;
		if (bodyType == "Dynamic") return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyType == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		DEBUG_CORE_ERROR("Unknown body type.");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		if (!entity.HasComponent<IDComponent>())
		{
			DEBUG_CORE_ERROR("Entity does not have an UUID.");
			return;
		}

		out << YAML::BeginMap;	// Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;	//TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;	//TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;	//SpriteRendererComponent

			auto& src = entity.GetComponent<SpriteRendererComponent>();
		
			if(src.Texture && std::filesystem::exists(src.Texture->GetPath()))
			{
				out << YAML::Key << "TexturePath" << YAML::Value << src.Texture->GetPath();

				// Save Subtexture parameters to create subtextures on deserialization.
				out << YAML::Key << "SubtextureCoords" << YAML::Value << src.SubtextureCoords;
				out << YAML::Key << "SubtextureCellSize" << YAML::Value << src.SubtextureCellSize;
				out << YAML::Key << "SubtextureSpriteSize" << YAML::Value << src.SubtextureSpriteSize;
			}
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			out << YAML::Key << "TilingFactor" << YAML::Value << src.TilingFactor;
			out << YAML::EndMap;	//SpriteRendererComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;	// Rigidbody2DComponent

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << Rigidbody2DBodyTypeTostring(rb2d.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;	// BoxCollider2DComponent

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;	//CameraComponent

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;
			out << YAML::Key << "Camera";
			out << YAML::BeginMap;	//Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;	//Camera

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.fixedAspectRatio;
			out << YAML::Key << "ClearColor" << YAML::Value << cc.ClearColor;

			out << YAML::EndMap;	//CameraComponent
		}

		if (entity.HasComponent<AudioSourceComponent>())
		{
			out << YAML::Key << "AudioSourceComponent";
			out << YAML::BeginMap;	//AudioSourceComponent

			auto& asc = entity.GetComponent<AudioSourceComponent>();

			if (asc.AudioSource && std::filesystem::exists(asc.AudioSource->GetAudioFilePath()))
			{
				out << YAML::Key << "AudioPath" << YAML::Value << asc.AudioSource->GetAudioFilePath();
			}
			
			out << YAML::Key << "PlayOnStart" << YAML::Value << asc.PlayOnStart;
			out << YAML::Key << "Loop" << YAML::Value << asc.Loop;
			out << YAML::Key << "Gain" << YAML::Value << asc.Gain;
			out << YAML::Key << "Pitch" << YAML::Value << asc.Pitch;

			out << YAML::EndMap;	//AudioSourceComponent
		}

		if (entity.HasComponent<AudioListenerComponent>())
		{
			out << YAML::Key << "AudioListenerComponent";
			out << YAML::BeginMap;	//AudioListenerComponent

			auto& ac = entity.GetComponent<AudioListenerComponent>();

			out << YAML::Key << "PositionSameAsEntity" << YAML::Value << ac.PositionSameAsEntity;
			out << YAML::Key << "Gain" << YAML::Value << ac.Gain;
			out << YAML::Key << "Position" << YAML::Value << ac.Position;
			out << YAML::Key << "Velocity" << YAML::Value << ac.Velocity;
			out << YAML::Key << "At" << YAML::Value << ac.At;
			out << YAML::Key << "Up" << YAML::Value << ac.Up;

			out << YAML::EndMap;	//AudioListenerComponent
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;	//NativeScriptComponent
			out << YAML::EndMap;	//NativeScriptComponent
		}

		out << YAML::EndMap;	// Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		// Extract Name From FilePath
		// Example: src/Scene/3DCube.odd -> 3DCube
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		std::string sceneName = filepath.substr(lastSlash, count);

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << sceneName.c_str();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{

	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"]; 
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities Always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspective((float)cameraProps["PerspectiveFOV"].as<float>(),
											 (float)cameraProps["PerspectiveNearClip"].as<float>(),
											 (float)cameraProps["PerspectiveFarClip"].as<float>());

					cc.Camera.SetOrthographic((float)cameraProps["OrthographicSize"].as<float>(),
											  (float)cameraProps["OrthographicNearClip"].as<float>(),
											  (float)cameraProps["OrthographicFarClip"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					cc.ClearColor = cameraComponent["ClearColor"].as<glm::vec4>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					
					if (spriteRendererComponent["TexturePath"])
					{
						src.Texture = Texture2D::Create(spriteRendererComponent["TexturePath"].as<std::string>());

						// Read Subtexture data.
						auto& subtextureCoords = spriteRendererComponent["SubtextureCoords"].as<glm::vec2>();
						auto& subtextureCellSize = spriteRendererComponent["SubtextureCellSize"].as<glm::vec2>();
						auto& subtextureSpriteSize = spriteRendererComponent["SubtextureSpriteSize"].as<glm::vec2>();

						if (subtextureCellSize.x > 0.0f && subtextureCellSize.y > 0.0f &&
							subtextureSpriteSize.x > 0.0f && subtextureSpriteSize.y > 0.0f)
						{
							// Create Subtexture only if the values are valid.
							src.SubtextureCoords = subtextureCoords;
							src.SubtextureCellSize = subtextureCellSize;
							src.SubtextureSpriteSize = subtextureSpriteSize;

							src.Subtexture = SubTexture2D::CreateFromCoords(src.Texture, src.SubtextureCoords, src.SubtextureCellSize, src.SubtextureSpriteSize);
						}
					}
						
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();

					rb2d.Type = Rigidbody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();

					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto audioSourceComponent = entity["AudioSourceComponent"];
				if (audioSourceComponent)
				{
					auto& asc = deserializedEntity.AddComponent<AudioSourceComponent>();

					if (audioSourceComponent["AudioPath"])
					{
						asc.AudioSource = AudioSource::Create(audioSourceComponent["AudioPath"].as<std::string>());
					}

					asc.PlayOnStart = audioSourceComponent["PlayOnStart"].as<bool>();
					asc.Loop = audioSourceComponent["Loop"].as<bool>();
					asc.Gain = audioSourceComponent["Gain"].as<float>();
					asc.Pitch = audioSourceComponent["Pitch"].as<float>();
				}

				auto audioListenerComponent = entity["AudioListenerComponent"];
				if (audioListenerComponent)
				{
					auto& ac = deserializedEntity.AddComponent<AudioListenerComponent>();

					ac.PositionSameAsEntity = audioListenerComponent["PositionSameAsEntity"].as<bool>();
					ac.Gain = audioListenerComponent["Gain"].as<float>();
					ac.Position = audioListenerComponent["Position"].as<glm::vec3>();
					ac.Velocity = audioListenerComponent["Velocity"].as<glm::vec3>();
					ac.At = audioListenerComponent["At"].as<glm::vec3>();
					ac.Up = audioListenerComponent["Up"].as<glm::vec3>();
				}

				auto nativeScriptComponent = entity["NativeScriptComponent"];
				if (nativeScriptComponent)
				{
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not Implemented.
		return false;
	}
}