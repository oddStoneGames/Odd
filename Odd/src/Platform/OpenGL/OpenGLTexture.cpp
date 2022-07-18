#include "oddpch.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace Odd
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
			DEBUG_CORE_ERROR("Failed To Load Image!");

		m_Width = width;
		m_Height = height;

		GLenum channelType;
		
		switch (channels)
		{
			case 1:		channelType = GL_RED; break;
			case 2:		channelType = GL_RG; break;
			case 3:		channelType = GL_RGB; break;
			case 4:		channelType = GL_RGBA; break;
			default:	channelType = GL_RGB; break;
		}

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, channelType, width, height, 0, channelType, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}