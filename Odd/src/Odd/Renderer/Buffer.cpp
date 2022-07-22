#include "oddpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Odd {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		ODD_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		
		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		ODD_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}

}