#include "oddpch.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace Odd
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_Format(GL_RGBA)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
			DEBUG_CORE_ERROR("Failed To Load Image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		
		switch (channels)
		{
			case 1:		internalFormat = GL_R8; dataFormat = GL_RED; break;
			case 2:		internalFormat = GL_RG8; dataFormat = GL_RG; break;
			case 3:		internalFormat = GL_RGB8; dataFormat = GL_RGB; break;
			case 4:		internalFormat = GL_RGBA8; dataFormat = GL_RGBA; break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		m_InternalFormat = internalFormat;
		m_Format = dataFormat;
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bytesPerPixel = 0;

		switch (m_Format)
		{
			case GL_RED:	bytesPerPixel = 1; break;
			case GL_RG:		bytesPerPixel = 2; break;
			case GL_RGB:	bytesPerPixel = 3; break;
			case GL_RGBA:	bytesPerPixel = 4; break;
		}

		if (size != m_Width * m_Height * bytesPerPixel)
		{
			DEBUG_CORE_ERROR("OpenGL Takes a Texture as a Single Data Block so Data Size Must be The Entire Texture Size!");
			return;
		}

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}