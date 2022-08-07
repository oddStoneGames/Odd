#include "oddpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Odd {

	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();
	
	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

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