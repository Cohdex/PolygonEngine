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
		RGB_16F
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

		static GLenum textureFormatToGLFormat(TextureFormat format);
		static GLint textureFormatToGLInternalFormat(TextureFormat format);
		static bool isFloatTextureFormat(TextureFormat format);
	};

	inline GLenum Texture::textureFormatToGLFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R_8:
		case TextureFormat::R_16F:
			return GL_R;
		case TextureFormat::RG_8:
		case TextureFormat::RG_16F:
			return GL_RG;
		case TextureFormat::RGB_8:
		case TextureFormat::RGB_16F:
			return GL_RGB;
		}
	}

	inline GLint Texture::textureFormatToGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::R_8:
			return GL_R8;
		case TextureFormat::RG_8:
			return GL_RG8;
		case TextureFormat::RGB_8:
			return GL_RGB8;
		case TextureFormat::R_16F:
			return GL_R16F;
		case TextureFormat::RG_16F:
			return GL_RG16F;
		case TextureFormat::RGB_16F:
			return GL_RGB16F;
		}
	}

	inline bool Texture::isFloatTextureFormat(TextureFormat format)
	{
		return format == TextureFormat::R_16F ||
			format == TextureFormat::RG_16F ||
			format == TextureFormat::RGB_16F;
	}
}
