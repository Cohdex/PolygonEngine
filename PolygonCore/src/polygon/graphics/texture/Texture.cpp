#include "stdafx.h"

namespace plgn
{
	Texture::Texture(GLenum textureTarget)
		: m_textureTarget(textureTarget)
	{
	}

	void Texture::destroy()
	{
		glDeleteTextures(1, &m_textureId);
		m_textureId = 0;
	}

	void Texture::bind() const
	{
		glBindTexture(m_textureTarget, m_textureId);
	}

	void Texture::bind(unsigned int textureUnit) const
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		bind();
	}
}
