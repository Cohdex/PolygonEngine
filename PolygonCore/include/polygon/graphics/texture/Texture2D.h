#pragma once

#include "Texture.h"

namespace plgn
{
	class Texture2D : public Texture
	{
	private:
		unsigned int m_width, m_height;
		unsigned char m_channels;

	public:
		Texture2D(unsigned int width, unsigned int height, unsigned char channels, void* data);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
	};
}
