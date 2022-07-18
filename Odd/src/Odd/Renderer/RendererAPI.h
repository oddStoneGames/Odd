#pragma once

#include "glm.hpp"
#include "VertexArray.h"

namespace Odd {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			DirectX = 2,
			Vulkan = 3
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}