#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace plgn
{
	Texture2D::Texture2D(int width, int height, TextureFormat format, void* data)
		: Texture(GL_TEXTURE_2D), m_width(width), m_height(height), m_format(format)
	{
		init(data);
	}

	Texture2D::Texture2D(const std::string& filename)
		: Texture(GL_TEXTURE_2D)
	{
		int comp;
		void* data;
		bool isHdr = stbi_is_hdr(filename.c_str());

		if (isHdr)
			data = stbi_loadf(filename.c_str(), &m_width, &m_height, &comp, 0);
		else
			data = stbi_load(filename.c_str(), &m_width, &m_height, &comp, 0);

		switch (comp)
		{
		case 1:
			m_format = isHdr ? TextureFormat::R_16F : TextureFormat::R_8;
			break;
		case 2:
			m_format = isHdr ? TextureFormat::RG_16F : TextureFormat::RG_8;
			break;
		case 3:
			m_format = isHdr ? TextureFormat::RGB_16F : TextureFormat::RGB_8;
			break;
		case 4:
			m_format = isHdr ? TextureFormat::RGBA_16F : TextureFormat::RGBA_8;
			break;
		}

		init(data);

		stbi_image_free(data);
	}

	void Texture2D::init(void* data)
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
