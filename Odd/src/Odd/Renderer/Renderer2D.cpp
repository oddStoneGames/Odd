#include "oddpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "RenderCommand.h"

namespace Odd
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		//Generate Vertex Array Object.
		s_Data->QuadVertexArray = VertexArray::Create();
		
		s_Data->QuadVertexArray->Bind();

		float vertices[] =
		{
			//Vertices			  
			-0.25f, -0.25f, 0.0f,
			 0.25f, -0.25f, 0.0f,
			 0.25f,  0.25f, 0.0f,
			-0.25f,  0.25f, 0.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		Odd::Ref<Odd::VertexBuffer> m_SquareVBO;

		//Generate Vertex Buffer Object.
		m_SquareVBO = Odd::VertexBuffer::Create(vertices, sizeof(vertices));

		Odd::Ref<Odd::IndexBuffer> m_SquareEBO;

		//Generate Element Buffer Object.
		m_SquareEBO = Odd::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		{
			//Generate Vertex Buffer Layout For All The Buffer Elements.
			Odd::BufferLayout layout = {
				{ Odd::ShaderDataType::Float3, "a_Pos"}
			};

			//Set The Layout For The Vertex Buffer.
			m_SquareVBO->SetLayout(layout);
		}

		s_Data->QuadVertexArray->AddVertexBuffer(m_SquareVBO);
		s_Data->QuadVertexArray->SetIndexBuffer(m_SquareEBO);
			  
		s_Data->QuadShader = Odd::Shader::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/Shaders/Square.glsl");
	}
	
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color)
	{
		// Bind Shader.
		s_Data->QuadShader->Bind();

		//Upload Data To The Shader.
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->QuadShader->SetMat4("u_Transform", transform);
		s_Data->QuadShader->SetFloat4("u_Color", color);

		// Bind Vertex Array.
		s_Data->QuadVertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		// Unbind Vertex Array.
		s_Data->QuadVertexArray->Unbind();

		// Unbind Shader.
		s_Data->QuadShader->Unbind();
	}
}