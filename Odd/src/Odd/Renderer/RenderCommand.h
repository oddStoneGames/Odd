#pragma once

#include "RendererAPI.h"

namespace Odd {

	class RenderCommand
	{
	public:
		RenderCommand() {}
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewport(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(xOffset, yOffset, width, height);
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}