#include "oddpch.h"
#include "OpenGLContext.h"

namespace Odd {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		if (windowHandle == nullptr)
			DEBUG_CORE_ERROR("Window Handle is Null");

		m_TriangleVAO = m_TriangleShaderID = 0;
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (gladStatus > 0)
			DEBUG_CORE_INFO("Glad Initialized Successfully!");
		else
			DEBUG_CORE_ERROR("Failed to Initialized Glad!");

		DEBUG_CORE_INFO("OpenGL Info:\n\t\t\t\tVendor:   {0}\n\t\t\t\tRenderer: {1}\n\t\t\t\tVersion:  {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

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

	/// <summary>
	/// Renders A Default Traingle With Some Color at Vertices.
	/// </summary>
	void OpenGLContext::RenderTriangle()
	{
		if (m_TriangleVAO == 0)
		{
			//Initialized Everything For The First Time.

			#pragma region VAO & VBO Initialization

			//Generate Vertex Array Object.
			glGenVertexArrays(1, &m_TriangleVAO);
			glBindVertexArray(m_TriangleVAO);

			float vertices[] = 
			{
				//Vertices				//Colors
				-0.1f, -0.15f, 0.0f,	1.0f, 1.0f, 0.0f,
				 0.1f, -0.15f, 0.0f,	0.0f, 1.0f, 1.0f,
				 0.0f,  0.15f, 0.0f,	1.0f, 0.0f, 1.0f
			};

			//Generate Vertex Buffer Object.
			m_TriangleVBO.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			{
				//Generate Vertex Buffer Layout For All The Buffer Elements.
				BufferLayout layout = {
					{ ShaderDataType::Float3, "pos"},
					{ ShaderDataType::Float3, "color"}
				};

				//Set The Layout For The Vertex Buffer.
				m_TriangleVBO->SetLayout(layout);
			}

			//Send Data to GPU
			uint32_t index = 0;
			
			// Cached Layout
			const auto& layout = m_TriangleVBO->GetLayout();

			for (const auto& element : layout)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetElementCount(), 
					ShaderDataTypeToOpenGLType(element.Type), 
					element.Normalized ? GL_TRUE : GL_FALSE, 
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
			}

			//UnBind Vertex Array Object.
			glBindVertexArray(0);

			#pragma endregion

			#pragma region Shader Initialization

			//Vertex Shader Code
			const char* vertexShaderCode = 
				R"(#version 460 core
				   layout(location = 0)in vec3 pos;
				   layout(location = 1)in vec3 color;
				   out vec3 FragColor;
				   void main() 
				   {
						gl_Position = vec4(pos, 1.0f);
						FragColor = color;
				   })";

			//Fragment Shader Code
			const char* fragmentShaderCode =
				R"(#version 460 core
				   in vec3 FragColor;
				   out vec4 Color;
				   void main() 
				   {
						Color = vec4(FragColor, 1.0f);
				   })";

			//Create Vertex Shader.
			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			//Insert Code.
			glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
			//Compile Shader.
			glCompileShader(vertexShader);
			//Log Compilation Error.
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Triangle Vertex Shader Failed To Compile!\n\t{0}", infoLog);
			}

			//Create Fragment Shader.
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			//Insert Code.
			glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
			//Compile Shader.
			glCompileShader(fragmentShader);
			//Log Compilation Error.
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Triangle Fragment Shader Failed To Compile!\n\t{0}", infoLog);
			}

			//Create Shader Program.
			m_TriangleShaderID = glCreateProgram();
			glAttachShader(m_TriangleShaderID, vertexShader);
			glAttachShader(m_TriangleShaderID, fragmentShader);
			glLinkProgram(m_TriangleShaderID);
			// check for linking errors
			glGetProgramiv(m_TriangleShaderID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_TriangleShaderID, 512, NULL, infoLog);
				DEBUG_CORE_ERROR("Triangle Shader Program Failed To Link!\n\t{0}", infoLog);
			}

			//Cleanup
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			#pragma endregion
			
		}

		//Bind VAO
		glBindVertexArray(m_TriangleVAO);

		//Bind Shader
		glUseProgram(m_TriangleShaderID);

		//Draw Triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind VAO
		glBindVertexArray(0);
	}

}