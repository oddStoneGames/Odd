#include "oddpch.h"
#include "Renderer.h"

namespace Odd {
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		// Bind Vertex Array.
		vertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(vertexArray);

		// Unbind Vertex Array.
		vertexArray->Unbind();
	}

}