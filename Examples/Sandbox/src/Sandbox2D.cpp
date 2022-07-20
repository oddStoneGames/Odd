#include <Odd.h>
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Odd::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		//Initialized Everything For The First Time.
		InitializeTriangle();
		InitializeSquare();
		m_CameraPosition = m_Camera.GetPosition();
		m_CameraRotation = m_Camera.GetRotation();
	}

	void OnUpdate(Odd::Timestep timestep) override
	{
		m_timeSinceStartup += timestep;

		if (Odd::Input::IsKeyPressed(Odd::Key::Left))
			m_CameraPosition.x -= m_CameraMovementSpeed * timestep;
		if (Odd::Input::IsKeyPressed(Odd::Key::Right))
			m_CameraPosition.x += m_CameraMovementSpeed * timestep;
		if (Odd::Input::IsKeyPressed(Odd::Key::Up))
			m_CameraPosition.y += m_CameraMovementSpeed * timestep;
		if (Odd::Input::IsKeyPressed(Odd::Key::Down))
			m_CameraPosition.y -= m_CameraMovementSpeed * timestep;

		if (Odd::Input::IsKeyPressed(Odd::Key::A))
			m_CameraRotation += m_CameraRotationSpeed * timestep;
		if (Odd::Input::IsKeyPressed(Odd::Key::D))
			m_CameraRotation -= m_CameraRotationSpeed * timestep;

		Odd::RenderCommand::SetClearColor(glm::vec4(sin(m_timeSinceStartup), cos(m_timeSinceStartup), 1, 1));
		Odd::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Odd::Renderer::BeginScene(m_Camera);
		
		m_Texture->Bind();

		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3(2.0f));
		//Draw Triangle
		Odd::Renderer::Submit(m_TriangleShader, m_TriangleVAO, transform);

		for (int i = 1; i <= 5; ++i)
		{
			transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.32f + 0.32f * i, 0.0f, 0.0f)); 
			transform = glm::scale(transform, glm::vec3(0.4f * i));

			m_SquareShader->Bind();

			m_SquareShader->SetFloat("intensity", 1.0f -  (i - 1) * 0.25f );

			//Draw Square
			Odd::Renderer::Submit(m_SquareShader, m_SquareVAO, transform);
		}

		Odd::Renderer::EndScene(); 
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Odd::Event& event) override
	{

	}

private:
	void InitializeTriangle()
	{
		#pragma region VAO & VBO Initialization

		//Generate Vertex Array Object.
		m_TriangleVAO = Odd::VertexArray::Create();
		m_TriangleVAO->Bind();

		float vertices[] =
		{
			//Vertices					//Texture Coordinates
			 0.05f, -0.08f,  0.0f,		0.0f, 0.0f,
			 0.21f, -0.08f,  0.0f,		1.0f, 0.0f,
			 0.13f,  0.08f,  0.0f,		0.5f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2
		};

		//Generate Vertex Buffer Object.
		Odd::Ref<Odd::VertexBuffer> m_TriangleVBO;
		m_TriangleVBO.reset(Odd::VertexBuffer::Create(vertices, sizeof(vertices)));

		//Generate Element Buffer Object.
		Odd::Ref<Odd::IndexBuffer> m_TriangleEBO;

		//Generate Element Buffer Object.
		m_TriangleEBO.reset(Odd::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		{
			//Generate Vertex Buffer Layout For All The Buffer Elements.
			Odd::BufferLayout layout = {
				{ Odd::ShaderDataType::Float3, "pos"},
				{ Odd::ShaderDataType::Float2, "texCoord"}
			};

			//Set The Layout For The Vertex Buffer.
			m_TriangleVBO->SetLayout(layout);
		}

		m_TriangleVAO->AddVertexBuffer(m_TriangleVBO);
		m_TriangleVAO->SetIndexBuffer(m_TriangleEBO);

		#pragma endregion

		#pragma region Shader Initialization

		m_TriangleShader = Odd::Shader::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/Shaders/DefaultTriangle.glsl");

		m_TriangleShader->Bind();
		m_TriangleShader->SetInt("baseColorTexture", 0);
	
		m_Texture = Odd::Texture2D::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/textures/Logo.png");

		#pragma endregion
	}

	void InitializeSquare()
	{
		#pragma region VAO & VBO Initialization

		//Generate Vertex Array Object.
		m_SquareVAO = Odd::VertexArray::Create();
		m_SquareVAO->Bind();

		float vertices[] =
		{
			//Vertices					//Texture Coordinates
			-0.21f, -0.08f,  0.0f,		0.0f, 0.0f,
			-0.05f, -0.08f,  0.0f,		1.0f, 0.0f,
			-0.05f,  0.08f,  0.0f,		1.0f, 1.0f,
			-0.21f,  0.08f,  0.0f,		0.0f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		Odd::Ref<Odd::VertexBuffer> m_SquareVBO;

		//Generate Vertex Buffer Object.
		m_SquareVBO.reset(Odd::VertexBuffer::Create(vertices, sizeof(vertices)));

		Odd::Ref<Odd::IndexBuffer> m_SquareEBO;

		//Generate Element Buffer Object.
		m_SquareEBO.reset(Odd:: IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		{
			//Generate Vertex Buffer Layout For All The Buffer Elements.
			Odd::BufferLayout layout = {
				{ Odd::ShaderDataType::Float3, "pos"},
				{ Odd::ShaderDataType::Float2, "texCoord"}
			};

			//Set The Layout For The Vertex Buffer.
			m_SquareVBO->SetLayout(layout);
		}

		m_SquareVAO->AddVertexBuffer(m_SquareVBO);
		m_SquareVAO->SetIndexBuffer(m_SquareEBO);


		#pragma endregion

		#pragma region Shader Initialization

		m_SquareShader = Odd::Shader::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/Shaders/DefaultSquare.glsl");

		m_SquareShader->Bind();
		m_SquareShader->SetInt("baseColorTexture", 0);

		#pragma endregion
	}
private:
	Odd::Ref<Odd::Shader>	   m_TriangleShader;
	Odd::Ref<Odd::VertexArray>  m_TriangleVAO;
	
	Odd::Ref<Odd::Shader>       m_SquareShader;
	Odd::Ref<Odd::VertexArray>  m_SquareVAO;

	Odd::Ref<Odd::Texture2D> m_Texture;

	Odd::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;
	float m_CameraMovementSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_timeSinceStartup = 0.0f;
};

class Sandbox2D : public Odd::Application
{
	public:
		Sandbox2D() 
		{ 
			PushLayer(new ExampleLayer());
		}
		~Sandbox2D() {}
};
 
Odd::Application* Odd::CreateApplication()
{
	return new Sandbox2D();
}