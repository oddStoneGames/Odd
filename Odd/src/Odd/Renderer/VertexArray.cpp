#include "oddpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Renderer.h"

namespace Odd {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}

		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}


}