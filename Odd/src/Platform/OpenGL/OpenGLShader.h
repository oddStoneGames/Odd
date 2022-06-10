#pragma once

#include "Odd/Renderer/Shader.h"
#include "glad/glad.h"

namespace Odd {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader() {}
		OpenGLShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr) 
		{
			CreateShader(vShaderPath, fShaderPath, gShaderPath);
		}
		~OpenGLShader() 
		{
			DestroyShader();
		}

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr) override;
		virtual void DestroyShader() override;
		
		virtual void SetBool(const char* name, const bool& value) override;
		virtual void SetUint(const char* name, const glm::uint32_t& value) override;

		virtual void SetInt(const char* name, const glm::int32_t& value) override;
		virtual void SetInt2(const char* name, const glm::ivec2& value)	 override;
		virtual void SetInt3(const char* name, const glm::ivec3& value)	 override;
		virtual void SetInt4(const char* name, const glm::ivec4& value)	 override;
		
		virtual void SetFloat(const char* name, const glm::float32_t& value) override;
		virtual void SetFloat2(const char* name, const glm::vec2& value)	 override;
		virtual void SetFloat3(const char* name, const glm::vec3& value)	 override;
		virtual void SetFloat4(const char* name, const glm::vec4& value)	 override;

		virtual void SetMat2(const char* name, const glm::mat2& value) override;
		virtual void SetMat3(const char* name, const glm::mat3& value) override;
		virtual void SetMat4(const char* name, const glm::mat4& value) override;
	private:
		void CheckError(const uint32_t& ID, bool shader);
	};

}