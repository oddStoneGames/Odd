#include "oddpch.h"
#include "UniformBuffer.h"

#include "Odd/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Odd {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    DEBUG_CORE_INFO("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		DEBUG_CORE_ERROR("Unknown Renderer API!");
		return nullptr;
	}

}