#include "oddpch.h"
#include "OpenGLContext.h"

namespace Odd {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		if (windowHandle == nullptr)
			DEBUG_CORE_ERROR("Window Handle is Null");

		m_TriangleShader = nullptr;
		m_TriangleVAO = nullptr;

		m_SquareShader = nullptr;
		m_SquareVAO = nullptr;
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
		if (m_TriangleVAO == nullptr)
		{
			//Initialized Everything For The First Time.

			#pragma region VAO & VBO Initialization

			//Generate Vertex Array Object.
			m_TriangleVAO.reset(VertexArray::Create());
			m_TriangleVAO->Bind();

			float vertices[] = 
			{
				//Vertices				//Colors
				-0.1f, -0.15f, 0.0f,	1.0f, 1.0f, 0.0f,
				 0.1f, -0.15f, 0.0f,	0.0f, 1.0f, 1.0f,
				 0.0f,  0.15f, 0.0f,	1.0f, 0.0f, 1.0f
			};

			uint32_t indices[] =
			{
				0, 1, 2
			};

			//Generate Vertex Buffer Object.
			std::shared_ptr<VertexBuffer> m_TriangleVBO;
			m_TriangleVBO.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			//Generate Element Buffer Object.
			std::shared_ptr<IndexBuffer> m_TriangleEBO;

			//Generate Element Buffer Object.
			m_TriangleEBO.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

			{
				//Generate Vertex Buffer Layout For All The Buffer Elements.
				BufferLayout layout = {
					{ ShaderDataType::Float3, "pos"},
					{ ShaderDataType::Float3, "color"}
				};

				//Set The Layout For The Vertex Buffer.
				m_TriangleVBO->SetLayout(layout);
			}

			m_TriangleVAO->AddVertexBuffer(m_TriangleVBO);
			m_TriangleVAO->SetIndexBuffer(m_TriangleEBO);

			#pragma endregion

			#pragma region Shader Initialization
			
			m_TriangleShader.reset(Shader::Create("D:/OddStoneGames/Odd/Odd/src/Platform/OpenGL/Shaders/DefaultTriangle.vs", "D:/OddStoneGames/Odd/Odd/src/Platform/OpenGL/Shaders/DefaultTriangle.fs"));

			#pragma endregion
			
		}

		//Draw Triangle
		Renderer::Submit(m_TriangleShader, m_TriangleVAO);
	}


	/// <summary>
	/// Renders A Default Square With Some Color at Vertices.
	/// </summary>
	void OpenGLContext::RenderSquare()
	{
		if (m_SquareVAO == nullptr)
		{
			//Initialized Everything For The First Time.

			#pragma region VAO & VBO Initialization

			//Generate Vertex Array Object.
			m_SquareVAO.reset(VertexArray::Create());
			m_SquareVAO->Bind();

			float vertices[] =
			{
				//Vertices				//Colors
				-0.5f, -0.15f, 0.0f,	1.0f, 1.0f, 0.0f,
				-0.3f, -0.15f, 0.0f,	0.0f, 1.0f, 1.0f,
				-0.3f,  0.15f, 0.0f,	1.0f, 0.0f, 1.0f,
				-0.5f,  0.15f, 0.0f,	1.0f, 1.0f, 1.0f
			};

			uint32_t indices[] =
			{
				0, 1, 2,
				2, 3, 0
			};

			std::shared_ptr<VertexBuffer> m_SquareVBO;

			//Generate Vertex Buffer Object.
			m_SquareVBO.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			std::shared_ptr<IndexBuffer> m_SquareEBO;

			//Generate Element Buffer Object.
			m_SquareEBO.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

			{
				//Generate Vertex Buffer Layout For All The Buffer Elements.
				BufferLayout layout = {
					{ ShaderDataType::Float3, "pos"},
					{ ShaderDataType::Float3, "color"}
				};

				//Set The Layout For The Vertex Buffer.
				m_SquareVBO->SetLayout(layout);
			}

			m_SquareVAO->AddVertexBuffer(m_SquareVBO);
			m_SquareVAO->SetIndexBuffer(m_SquareEBO);


			#pragma endregion

			#pragma region Shader Initialization

			m_SquareShader.reset(Shader::Create("D:/OddStoneGames/Odd/Odd/src/Platform/OpenGL/Shaders/DefaultSquare.vs", "D:/OddStoneGames/Odd/Odd/src/Platform/OpenGL/Shaders/DefaultSquare.fs"));

			#pragma endregion

		}

		//Draw Square
		Renderer::Submit(m_SquareShader, m_SquareVAO);
	}
}