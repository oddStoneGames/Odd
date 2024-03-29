#include "oddpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Odd {

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
			case Odd::ShaderDataType::None:		return 0;
			case Odd::ShaderDataType::Bool:		return GL_BOOL;
			case Odd::ShaderDataType::Int:		return GL_INT;
			case Odd::ShaderDataType::Int2:		return GL_INT;
			case Odd::ShaderDataType::Int3:		return GL_INT;
			case Odd::ShaderDataType::Int4:		return GL_INT;
			case Odd::ShaderDataType::Float:	return GL_FLOAT;
			case Odd::ShaderDataType::Float2:	return GL_FLOAT;
			case Odd::ShaderDataType::Float3:	return GL_FLOAT;
			case Odd::ShaderDataType::Float4:	return GL_FLOAT;
			case Odd::ShaderDataType::Mat3:		return GL_FLOAT;
			case Odd::ShaderDataType::Mat4:		return GL_FLOAT;
		}

		DEBUG_CORE_ERROR("Shader Data Type Unknown!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ODD_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ODD_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		ODD_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		//Send Data to GPU
		uint32_t index = 0;

		// Cached Layout
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetElementCount(),
					ShaderDataTypeToOpenGLType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, element.GetElementCount(),
					ShaderDataTypeToOpenGLType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			default:
				break;
			}
			
		}

		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		ODD_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
		glBindVertexArray(0);
	}

}