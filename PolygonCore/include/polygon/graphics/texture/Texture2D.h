#pragma once

#include <GL/glew.h>

namespace plgn
{
	class Texture2D
	{
	private:
		GLuint m_texture;
		unsigned int m_width, m_height;
		unsigned char m_channels;

	public:
		Texture2D(unsigned int width, unsigned int height, unsigned char channels, void* data);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		void destroy();

		void bind() const;
	};
}
