#pragma once

#include "Texture.h"

#include <string>

namespace plgn
{
	class Texture2D : public Texture
	{
	private:
		int m_width, m_height;
		TextureFormat m_format;

		void init(void* data);

	public:
		Texture2D(int width, int height, TextureFormat format, void* data);
		Texture2D(const std::string& filename);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
	};
}
