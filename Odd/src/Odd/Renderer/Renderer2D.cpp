#include "oddpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Odd
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID = -1;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 100000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Get From Render Capability

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadTextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 - White Texture

		glm::vec4 QuadVertexPositions[4];
		glm::vec2 QuadTexCoords[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ODD_PROFILE_FUNCTION();

		#pragma region Generate Quad Vertex Array

		//Generate Vertex Array Object.
		s_Data.QuadVertexArray = VertexArray::Create();
		
		s_Data.QuadVertexArray->Bind();

		//Generate Vertex Buffer Object.
		s_Data.QuadVertexBuffer = Odd::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		//Generate Vertex Buffer Layout For All The Buffer Elements.
		s_Data.QuadVertexBuffer->SetLayout({
				{ Odd::ShaderDataType::Float3, "a_Pos"},
				{ Odd::ShaderDataType::Float4, "a_Color"},
				{ Odd::ShaderDataType::Float2, "a_TexCoord"},
				{ Odd::ShaderDataType::Float, "a_TexIndex"},
				{ Odd::ShaderDataType::Float, "a_TilingFactor"},
				{ Odd::ShaderDataType::Int,   "a_EntityID"}
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		//Generate Element Buffer Object.
		Odd::Ref<Odd::IndexBuffer> quadEBO = Odd::IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadEBO);
		delete[] quadIndices;

		#pragma endregion

		#pragma region Generate Shader, Texture & Renderer2D Data

		s_Data.WhiteTexture = Odd::Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadTextureShader = Odd::Shader::Create("D:/OddStoneGames/Odd/Examples/Sandbox/src/Shaders/TextureQuad.glsl");
		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set Texture Slots to 0.
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadTexCoords[0] = { 0.0f, 0.0f };
		s_Data.QuadTexCoords[1] = { 1.0f, 0.0f };
		s_Data.QuadTexCoords[2] = { 1.0f, 1.0f };
		s_Data.QuadTexCoords[3] = { 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);

		#pragma endregion

	}
	
	void Renderer2D::Shutdown()
	{
		ODD_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ODD_PROFILE_FUNCTION();

		s_Data.QuadTextureShader->Bind();
		s_Data.QuadTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		ODD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		ODD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}
	
	void Renderer2D::EndScene()
	{
		ODD_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		ODD_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind Vertex Array.
		s_Data.QuadVertexArray->Bind();
		// Bind Shader.
		s_Data.QuadTextureShader->Bind();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;

		// Unbind Shader.
		s_Data.QuadTextureShader->Unbind();
		// Unbind Vertex Array.
		s_Data.QuadVertexArray->Unbind();

	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	#pragma region Draw Quad

	#pragma region Colored Quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			NextBatch();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	#pragma endregion

	#pragma region Textured Quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture> texture, const float tilingFactor, int entityID)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			NextBatch();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	#pragma endregion

	#pragma region Sub Textured Quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subtexture, const float tilingFactor, int entityID)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			NextBatch();

		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	#pragma endregion

	#pragma region Colored Textured Quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor, int entityID)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	#pragma endregion

	#pragma region Colored Sub Textured Quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, subtexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		DrawQuad(transform, color, subtexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor, int entityID)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			NextBatch();

		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	#pragma endregion

	#pragma endregion

	#pragma region Draw Rotated Quad

	#pragma region Colored Quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	#pragma endregion

	#pragma region Textured Quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture> texture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor);
	}

	#pragma endregion

	#pragma region Sub Textured Quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, subtexture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tilingFactor);
	}

	#pragma endregion

	#pragma region Colored Textured Quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<Texture> texture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color, texture, tilingFactor);
	}

	#pragma endregion

	#pragma region Colored Sub Textured Quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color, subtexture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4 color, const Ref<SubTexture2D> subtexture, const float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, subtexture, tilingFactor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, uint32_t entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Color, src.Texture, src.TilingFactor, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	#pragma endregion

	#pragma endregion

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}