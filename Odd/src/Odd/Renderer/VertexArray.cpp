#include "oddpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Odd {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}


}