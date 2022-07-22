#include "oddpch.h"
#include "Shader.h"

#include "Odd/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Odd {
	Ref<Shader> Shader::Create(const std::string& shaderPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DEBUG_CORE_ERROR("Renderer API is None."); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(shaderPath);
		case RendererAPI::API::DirectX:	return nullptr;
		case RendererAPI::API::Vulkan:	return nullptr;
		}

		DEBUG_CORE_ERROR("Renderer API Undefined!!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DEBUG_CORE_ERROR("Renderer API is None."); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShader>(name, vShaderSource, fShaderSource, gShaderSource);
			case RendererAPI::API::DirectX:	return nullptr;
			case RendererAPI::API::Vulkan:	return nullptr;
		}

		DEBUG_CORE_ERROR("Renderer API Undefined!!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		if (Exists(name))
		{
			DEBUG_CORE_ERROR("Shader Already Exists in Shader Library!");
			return;
		}
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (!Exists(name))
		{
			DEBUG_CORE_ERROR("Shader Doesn't Exists in Shader Library!");
			return nullptr; 
		}
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}