#include "oddpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <iostream>

namespace Odd {

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::CreateShader(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		bool isGeometryShaderPathNotNull = gShaderPath != nullptr;

		try
		{
			// open files
			vShaderFile.open(vShaderPath);
			fShaderFile.open(fShaderPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (isGeometryShaderPathNotNull)
			{
				gShaderFile.open(gShaderPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			DEBUG_CORE_ERROR("File Not Successfully Read: {0}\n", e.what());
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckError(vertex, true);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckError(fragment, true);
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (isGeometryShaderPathNotNull)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			CheckError(geometry, true);
		}
		// shader Program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertex);
		glAttachShader(m_RendererID, fragment);
		if (isGeometryShaderPathNotNull)
			glAttachShader(m_RendererID, geometry);
		glLinkProgram(m_RendererID);
		CheckError(m_RendererID, false);

		// 3. Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (isGeometryShaderPathNotNull)
			glDeleteShader(geometry);
	}

	void OpenGLShader::DestroyShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::SetBool(const char* name, const bool& value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name), (int)value);
	}

	void OpenGLShader::SetUint(const char* name, const glm::uint32_t& value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name), (unsigned int)value);
	}

	void OpenGLShader::SetInt(const char* name, const glm::int32_t& value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name), (int)value);
	}

	void OpenGLShader::SetInt2(const char* name, const glm::ivec2& value)
	{
		glUniform2iv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetInt3(const char* name, const glm::ivec3& value)
	{
		glUniform3iv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetInt4(const char* name, const glm::ivec4& value)
	{
		glUniform4iv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetFloat(const char* name, const float& value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name), value);
	}

	void OpenGLShader::SetFloat2(const char* name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetFloat3(const char* name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetFloat4(const char* name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(m_RendererID, name), 1, &value[0]);
	}

	void OpenGLShader::SetMat2(const char* name, const glm::mat2& value)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_RendererID, name), 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name), 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name), 1, GL_FALSE, &value[0][0]);
	}

	void OpenGLShader::CheckError(const uint32_t& ID, bool shader)
	{
		GLint success;
		GLchar infoLog[512];
		if (shader)
		{
			glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(ID, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Compilation Failed For Shader: {0}\n", infoLog);
			}
		}
		else
		{
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Linking Failed For Shader Program: {0}\n", infoLog);
			}
		}
	}

}