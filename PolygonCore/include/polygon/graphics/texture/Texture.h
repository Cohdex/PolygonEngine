#pragma once

#include <GL/glew.h>

namespace plgn
{
	enum TextureFormat
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

		GLuint getTextureHandle() const { return m_textureId; }

		void destroy();

		void bind() const;
		void bind(unsigned int textureUnit) const;
	};
}
