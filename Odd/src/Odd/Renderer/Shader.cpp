#include "oddpch.h"
#include "Shader.h"

#include "Odd/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Odd {
	Ref<Shader> Shader::Create(const char* shaderPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DEBUG_CORE_ERROR("Renderer API is None."); return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(shaderPath);
		case RendererAPI::API::DirectX:	return nullptr;
		case RendererAPI::API::Vulkan:	return nullptr;
		}

		DEBUG_CORE_ERROR("Renderer API Undefined!!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const char* vShaderSource, const char* fShaderSource, const char* gShaderSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_ERROR("Renderer API is None."); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(vShaderSource, fShaderSource, gShaderSource);
			case RendererAPI::API::DirectX:	return nullptr;
			case RendererAPI::API::Vulkan:	return nullptr;
		}

		DEBUG_CORE_ERROR("Renderer API Undefined!!");
		return nullptr;
	}

}