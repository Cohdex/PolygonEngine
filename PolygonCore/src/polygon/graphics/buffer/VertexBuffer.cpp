#include "stdafx.h"

namespace plgn
{
	VertexBuffer::VertexBuffer(const std::vector<GLfloat>& buffer)
		: m_count(buffer.size()), m_elementSize(1)
	{
		init(buffer.data());
	}

	VertexBuffer::VertexBuffer(const std::vector<glm::vec2>& buffer)
		: m_count(buffer.size()), m_elementSize(2)
	{
		init((const GLfloat*)buffer.data());
	}

	VertexBuffer::VertexBuffer(const std::vector<glm::vec3>& buffer)
		: m_count(buffer.size()), m_elementSize(3)
	{
		init((const GLfloat*)buffer.data());
	}

	VertexBuffer::VertexBuffer(const std::vector<glm::vec4>& buffer)
		: m_count(buffer.size()), m_elementSize(4)
	{
		init((const GLfloat*)buffer.data());
	}

	void VertexBuffer::init(const GLfloat* data)
	{
		glGenBuffers(1, &m_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(GL_ARRAY_BUFFER, m_count * getStride(), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::destroy()
	{
		glDeleteBuffers(1, &m_vboId);
		m_vboId = 0;
	}
}
