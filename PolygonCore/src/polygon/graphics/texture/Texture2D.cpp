#include "stdafx.h"

namespace plgn
{
	Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned char channels, void* data)
		: m_width(width), m_height(height), m_channels(channels)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::destroy()
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}

	void Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}
