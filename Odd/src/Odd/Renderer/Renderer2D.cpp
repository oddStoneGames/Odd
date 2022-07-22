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
		Ref<Shader> QuadTextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		#pragma region Generate Quad Vertex Array

		//Generate Vertex Array Object.
		s_Data->QuadVertexArray = VertexArray::Create();
		
		s_Data->QuadVertexArray->Bind();

		float vertices[] =
		{
			//Vertices				//Texture Coordinates
			-0.25f, -0.25f, 0.0f,   0.0f, 0.0f,
			 0.25f, -0.25f, 0.0f,	1.0f, 0.0f,
			 0.25f,  0.25f, 0.0f,	1.0f, 1.0f,
			-0.25f,  0.25f, 0.0f,	0.0f, 1.0f
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
				{ Odd::ShaderDataType::Float3, "a_Pos"},
				{ Odd::ShaderDataType::Float2, "a_TexCoord"}
			};

			//Set The Layout For The Vertex Buffer.
			m_SquareVBO->SetLayout(layout);
		}

		s_Data->QuadVertexArray->AddVertexBuffer(m_SquareVBO);
		s_Data->QuadVertexArray->SetIndexBuffer(m_SquareEBO);
		
		#pragma endregion

		#pragma region Generate Shader Data & Texture Data

		s_Data->WhiteTexture = Odd::Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->QuadTextureShader = Odd::Shader::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/Shaders/TextureQuad.glsl");
		s_Data->QuadTextureShader->Bind();
		s_Data->QuadTextureShader->SetInt("u_Texture", 0);

		#pragma endregion

	}
	
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadTextureShader->Bind();
		s_Data->QuadTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->QuadTextureShader->SetInt("u_Texture", 0);
		s_Data->QuadTextureShader->SetFloat2("u_TextureScale", glm::vec2(1.0f));
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
		s_Data->QuadTextureShader->Bind();

		//Upload Data To The Shader.
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->QuadTextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadTextureShader->SetFloat4("u_Color", color);

		// Bind Default White Texture.
		s_Data->WhiteTexture->Bind();

		// Bind Vertex Array.
		s_Data->QuadVertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		// Unbind Vertex Array.
		s_Data->QuadVertexArray->Unbind();

		// Unbind Shader.
		s_Data->QuadTextureShader->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec2 textureScale)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const glm::vec2 textureScale)
	{
		// Bind Shader.
		s_Data->QuadTextureShader->Bind();

		//Upload Data To The Shader.
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->QuadTextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadTextureShader->SetFloat2("u_TextureScale", textureScale);
		
		// Bind Texture.
		texture->Bind();

		// Bind Vertex Array.
		s_Data->QuadVertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		// Unbind Vertex Array.
		s_Data->QuadVertexArray->Unbind();

		// Unbind Shader.
		s_Data->QuadTextureShader->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const glm::vec2 textureScale)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture, textureScale);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const glm::vec2 textureScale)
	{
		// Bind Shader.
		s_Data->QuadTextureShader->Bind();

		//Upload Data To The Shader.
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, 1.0f));

		s_Data->QuadTextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadTextureShader->SetFloat2("u_TextureScale", textureScale);
		s_Data->QuadTextureShader->SetFloat4("u_Color", color);

		// Bind Texture.
		texture->Bind();

		// Bind Vertex Array.
		s_Data->QuadVertexArray->Bind();

		// initiate a Draw Call.
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		// Unbind Vertex Array.
		s_Data->QuadVertexArray->Unbind();

		// Unbind Shader.
		s_Data->QuadTextureShader->Unbind();
	}
}