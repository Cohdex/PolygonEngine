#pragma once

#include <GL/glew.h>

#include <vector>

namespace plgn
{
	class IndexBuffer
	{
	private:
		GLuint m_eboId;
		GLint m_count;

	public:
		IndexBuffer(const std::vector<GLuint>& indices);

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		inline GLuint getHandle() const { return m_eboId; }
		inline GLint getCount() const { return m_count; }

		void destroy();
	};
}
