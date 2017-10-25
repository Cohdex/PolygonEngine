#include "stdafx.h"

namespace plgn
{
	Texture2D::Texture2D(unsigned int width, unsigned int height, TextureFormat format, void* data)
		: Texture(GL_TEXTURE_2D), m_width(width), m_height(height), m_format(format)
	{
		GLenum dataType;
		if (isFloatTextureFormat(m_format))
			dataType = GL_FLOAT;
		else
			dataType = GL_UNSIGNED_BYTE;

		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormatToGLInternalFormat(m_format), m_width, m_height, 0,
			textureFormatToGLFormat(m_format), dataType, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		if (GLEW_EXT_texture_filter_anisotropic)
		{
			float aniso;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
