#include "stdafx.h"

namespace plgn
{
	VertexArray::VertexArray(GLsizei count, DrawMode drawMode /*= TRIANGLES*/)
		: m_count(count), m_drawMode(drawMode)
	{
		glGenVertexArrays(1, &m_vaoId);
	}

	VertexArray::~VertexArray()
	{
		for (VertexBuffer* buffer : m_vertexBuffers)
		{
			delete buffer;
		}
		m_vertexBuffers.clear();
	}

	void VertexArray::addVertexBuffer(VertexBuffer* buffer, GLuint attributeIndex)
	{
		glBindVertexArray(m_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, buffer->getHandle());
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, buffer->getStride(), GL_FLOAT, GL_FALSE, buffer->getStride() * sizeof(GLfloat), (void*)0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_vertexBuffers.push_back(buffer);
	}

	void VertexArray::setIndexBuffer(IndexBuffer* buffer)
	{
		glBindVertexArray(m_vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->getHandle());
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_indexBuffer.reset(buffer);
	}

	void VertexArray::draw() const
	{
		GLenum glDrawMode;
		switch (m_drawMode)
		{
		case DrawMode::POINTS:
			glDrawMode = GL_POINTS;
			break;
		case DrawMode::LINES:
			glDrawMode = GL_LINES;
			break;
		case DrawMode::TRIANGLES:
			glDrawMode = GL_TRIANGLES;
			break;
		case DrawMode::TRIANGLE_STRIP:
			glDrawMode = GL_TRIANGLE_STRIP;
			break;
		case DrawMode::TRIANGLE_FAN:
			glDrawMode = GL_TRIANGLE_FAN;
			break;
		}

		glBindVertexArray(m_vaoId);
		if (m_indexBuffer == nullptr)
		{
			glDrawArrays(glDrawMode, 0, m_count);
		}
		else
		{
			glDrawElements(glDrawMode, m_count, GL_UNSIGNED_INT, nullptr);
		}
		glBindVertexArray(0);
	}

	void VertexArray::destroy()
	{
		glDeleteVertexArrays(1, &m_vaoId);
		m_vaoId = 0;

		for (VertexBuffer* buffer : m_vertexBuffers)
		{
			buffer->destroy();
		}
		
		if (m_indexBuffer != nullptr)
		{
			m_indexBuffer->destroy();
		}
	}
}
