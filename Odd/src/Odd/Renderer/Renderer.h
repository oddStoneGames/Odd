#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"

#include "Odd/Renderer/Shader.h"
#include "Odd/Renderer/OrthographicCamera.h"

namespace Odd {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}