#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Odd
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
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

		static void FlushAndReset();
	};
}