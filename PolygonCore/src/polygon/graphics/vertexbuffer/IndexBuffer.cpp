#include "stdafx.h"

namespace plgn
{
	IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices)
		: m_count(indices.size())
	{
		glGenBuffers(1, &m_eboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::destroy()
	{
		glDeleteBuffers(1, &m_eboId);
		m_eboId = 0;
	}
}
