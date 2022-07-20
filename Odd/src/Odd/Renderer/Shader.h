#pragma once

#include <unordered_map>
#include "glm.hpp"

namespace Odd
{
	class Shader
	{
	public:
		virtual ~Shader() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const std::string& GetName() const = 0;

		virtual void CreateShader(const std::string& shaderPath) = 0;
		virtual void CreateShader(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource = nullptr) = 0;
		virtual void DestroyShader() = 0;

		virtual void SetBool(const char* name, const bool& value) = 0;
		virtual void SetUint(const char* name, const glm::uint32_t& value) = 0;
		
		virtual void SetInt (const char* name, const glm::int32_t& value) = 0;
		virtual void SetInt2(const char* name, const glm::ivec2& value) = 0;
		virtual void SetInt3(const char* name, const glm::ivec3& value) = 0;
		virtual void SetInt4(const char* name, const glm::ivec4& value) = 0;
		
		virtual void SetFloat (const char* name, const float& value) = 0;
		virtual void SetFloat2(const char* name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const char* name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const char* name, const glm::vec4& value) = 0;
		
		virtual void SetMat2(const char* name, const glm::mat2& value) = 0;
		virtual void SetMat3(const char* name, const glm::mat3& value) = 0;
		virtual void SetMat4(const char* name, const glm::mat4& value) = 0;

		uint32_t& GetID() { return m_RendererID; }
		
		static Ref<Shader> Create(const std::string& shaderPath);
		static Ref<Shader> Create(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource = nullptr);

	protected:
		uint32_t m_RendererID;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		bool Exists(const std::string& name) const;
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}