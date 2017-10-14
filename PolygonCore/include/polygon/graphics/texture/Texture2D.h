#pragma once

#include "Texture.h"

namespace plgn
{
	class Texture2D : public Texture
	{
	private:
		const unsigned int m_width, m_height;
		const TextureFormat m_format;

	public:
		Texture2D(unsigned int width, unsigned int height, TextureFormat format, void* data);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
	};
}
