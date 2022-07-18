#include "oddpch.h"
#include "Renderer.h"

namespace Odd {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		// Bind Shader.
		shader->Bind();

		//Upload Scene Data To The Shader.
		shader->SetMat4("viewProjection", m_SceneData->viewProjectionMatrix);
		shader->SetMat4("transform", transform);

		// Bind Vertex Array.
		vertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(vertexArray);

		// Unbind Vertex Array.
		vertexArray->Unbind();

		// Unbind Shader.
		shader->Unbind();
	}

}