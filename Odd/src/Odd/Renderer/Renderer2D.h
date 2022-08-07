#pragma once

#include "Camera.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Odd/Scene/SceneCamera.h"
#include "EditorCamera.h"
#include "Odd/Scene/Components.h"

namespace Odd
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);

		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture> texture, const float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor = 1.0f, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f, int entityID = -1);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor = 1.0f);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, uint32_t entityID);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}