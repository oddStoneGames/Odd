#include "oddpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <iostream>

namespace Odd {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vert") return GL_VERTEX_SHADER;
		if (type == "frag") return GL_FRAGMENT_SHADER;
		if (type == "geom") return GL_GEOMETRY_SHADER;

		DEBUG_CORE_ERROR("Unknown Shader Type '{0}'", type);
		return 0;
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string source;

		std::ifstream File;

		// ensure ifstream object can throw exceptions:
		File.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open file
			File.open(filePath);
			std::stringstream FileStream;

			// read file's buffer contents into streams
			FileStream << File.rdbuf();
			// close file handler
			File.close();

			// convert stream into string
			source = FileStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			DEBUG_CORE_ERROR("File at {0} Not Successfully Read: {1}\n", filePath, e.what());
		}

		return source;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == std::string::npos)
			{
				DEBUG_CORE_ERROR("Syntax Error");
				return shaderSources;
			}

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			if (!ShaderTypeFromString(type))
			{
				DEBUG_CORE_ERROR("Invalid Shader Type Specified!");
				return shaderSources;
			}

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		
		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		
		if (shaderSources.size() > 2)
			DEBUG_CORE_ERROR("We Only Support 2 Shaders at the moment.");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCode = source.c_str();
			glShaderSource(shader, 1, &sourceCode, 0);

			glCompileShader(shader);
			bool isCompiled = CheckError(shader, true);
			if (!isCompiled)
			{
				glDeleteShader(shader);
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);
		bool isLinked = CheckError(program, false);
		if (!isLinked)
		{
			glDeleteProgram(program);
			for (auto id : glShaderIDs)
				glDeleteShader(id);

		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::CreateShader(const std::string& shaderPath)
	{
		// Retrieve the Raw Shader Source Code from shader file path.
		std::string rawShaderCode = ReadFile(shaderPath);

		// Grab Vertex, Fragment & Geometry Code From Raw Shader Code.
		auto shaderSourceCodes = PreProcess(rawShaderCode);

		// Compile The Shader Finally.
		Compile(shaderSourceCodes);

		// Extract Name From ShaderPath
		// Example: src/Shaders/DefaultTriangle.glsl
		auto lastSlash = shaderPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = shaderPath.rfind(".");  
		auto count = lastDot == std::string::npos ? shaderPath.size() - lastSlash : lastDot - lastSlash;
		m_Name = shaderPath.substr(lastSlash, count);
	}

	void OpenGLShader::CreateShader(const std::string& name, const char* vShaderSource, const char* fShaderSource, const char* gShaderSource)
	{
		if (vShaderSource == nullptr || fShaderSource == nullptr)
		{
			DEBUG_CORE_ERROR("Vertex Shader Or Fragment Shader Can Never Be Null While Creating a Shader!");
			return;
		}

		bool isGeometryShaderNotNull = gShaderSource != nullptr;

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vShaderSource;
		sources[GL_FRAGMENT_SHADER] = fShaderSource;
		if(isGeometryShaderNotNull)
			sources[GL_GEOMETRY_SHADER] = gShaderSource;

		// Compile Shaders
		Compile(sources);

		m_Name = name;
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

	bool OpenGLShader::CheckError(const uint32_t& ID, bool shader)
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
				return false;
			}
		}
		else
		{
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Linking Failed For Shader Program: {0}\n", infoLog);
				return false;
			}
		}
		return true;
	}

}