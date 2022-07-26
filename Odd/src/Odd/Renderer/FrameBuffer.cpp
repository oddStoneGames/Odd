#include "oddpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Odd
{
	Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specs)
	{
		ODD_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(specs);
		}

		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}
}