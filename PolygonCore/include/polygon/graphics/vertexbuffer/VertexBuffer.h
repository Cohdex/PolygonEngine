#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vector>

namespace plgn
{
	class VertexBuffer
	{
	private:
		GLuint m_vboId;
		GLint m_size;
		GLint m_stride;

		void init(const GLfloat* data);

	public:
		VertexBuffer(const std::vector<GLfloat>& buffer);
		VertexBuffer(const std::vector<glm::vec2>& buffer);
		VertexBuffer(const std::vector<glm::vec3>& buffer);
		VertexBuffer(const std::vector<glm::vec4>& buffer);

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer operator=(const VertexBuffer&) = delete;

		inline GLuint getHandle() const { return m_vboId; }
		inline GLint getSize() const { return m_size; }
		inline GLint getStride() const { return m_stride; }

		void destroy();
	};
}
