#include "oddpch.h"
#include "Shader.h"

#include "Odd/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Odd {

	Shader* Shader::Create(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_ERROR("Renderer API is None."); return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLShader(vShaderPath, fShaderPath, gShaderPath);
			case RendererAPI::API::DirectX:	return nullptr;
			case RendererAPI::API::Vulkan:	return nullptr;
		}

		DEBUG_CORE_ERROR("Renderer API Undefined!!");
		return nullptr;
	}

}