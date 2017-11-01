#pragma once

#include <GL/glew.h>

namespace plgn
{
	enum class TextureFormat
	{
		R_8,
		RG_8,
		RGB_8,
		R_16F,
		RG_16F,
		RGB_16F,
		RGBA_8,
		RGBA_16F
	};

	class Texture
	{
	protected:
		const GLenum m_textureTarget;
		GLuint m_textureId;

		Texture(GLenum textureTarget);

	public:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		GLuint getHandle() const { return m_textureId; }

		void destroy();

		void bind() const;
		void bind(unsigned int textureUnit) const;

		static GLint getGLInternalFormat(TextureFormat format);
		static GLenum getGLFormat(TextureFormat format);
		static GLenum getGLDataType(TextureFormat format);
	};

	inline GLint Texture::getGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R_8:
			return GL_R8;
		case TextureFormat::RG_8:
			return GL_RG8;
		case TextureFormat::RGB_8:
			return GL_RGB8;
		case TextureFormat::RGBA_8:
			return GL_RGBA8;
		case TextureFormat::R_16F:
			return GL_R16F;
		case TextureFormat::RG_16F:
			return GL_RG16F;
		case TextureFormat::RGB_16F:
			return GL_RGB16F;
		case TextureFormat::RGBA_16F:
			return GL_RGBA16F;
		default:
			return -1;
		}
	}

	inline GLenum Texture::getGLFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R_8:
		case TextureFormat::R_16F:
			return GL_RED;
		case TextureFormat::RG_8:
		case TextureFormat::RG_16F:
			return GL_RG;
		case TextureFormat::RGB_8:
		case TextureFormat::RGB_16F:
			return GL_RGB;
		case TextureFormat::RGBA_8:
		case TextureFormat::RGBA_16F:
			return GL_RGBA;
		default:
			return -1;
		}
	}

	inline GLenum Texture::getGLDataType(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R_8:
		case TextureFormat::RG_8:
		case TextureFormat::RGB_8:
		case TextureFormat::RGBA_8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::R_16F:
		case TextureFormat::RG_16F:
		case TextureFormat::RGB_16F:
		case TextureFormat::RGBA_16F:
			return GL_FLOAT;
		default:
			return -1;
		}
	}
}
