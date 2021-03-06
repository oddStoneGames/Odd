#pragma once

#include "Odd/Renderer/Shader.h"
#include "glad/glad.h"

namespace Odd {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader() {}
		OpenGLShader(const std::string& shaderPath)
		{
			CreateShader(shaderPath);
		}
		OpenGLShader(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource = nullptr) 
		{
			CreateShader(name, vShaderSource, fShaderSource, gShaderSource);
		}
		~OpenGLShader() 
		{
			DestroyShader();
		}

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void CreateShader(const std::string& shaderPath) override;
		virtual void CreateShader(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource = nullptr) override;
		virtual void DestroyShader() override;
		
		virtual void SetBool(const char* name, const bool& value) override;
		virtual void SetUint(const char* name, const glm::uint32_t& value) override;

		virtual void SetInt(const char* name, const glm::int32_t& value) override;
		virtual void SetInt2(const char* name, const glm::ivec2& value)	 override;
		virtual void SetInt3(const char* name, const glm::ivec3& value)	 override;
		virtual void SetInt4(const char* name, const glm::ivec4& value)	 override;
		virtual void SetIntArray(const char* name, int* values, uint32_t count) override;

		virtual void SetFloat(const char* name, const float& value) override;
		virtual void SetFloat2(const char* name, const glm::vec2& value)	 override;
		virtual void SetFloat3(const char* name, const glm::vec3& value)	 override;
		virtual void SetFloat4(const char* name, const glm::vec4& value)	 override;

		virtual void SetMat2(const char* name, const glm::mat2& value) override;
		virtual void SetMat3(const char* name, const glm::mat3& value) override;
		virtual void SetMat4(const char* name, const glm::mat4& value) override;
	private:
		std::string m_Name;
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
		bool CheckError(const uint32_t& ID, bool shader);
	};

}