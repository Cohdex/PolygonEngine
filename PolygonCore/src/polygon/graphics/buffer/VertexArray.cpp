#include "stdafx.h"

namespace plgn
{
	VertexArray::~VertexArray()
	{
		for (VertexBuffer* buffer : m_vertexBuffers)
		{
			delete buffer;
		}
		m_vertexBuffers.clear();
	}

	void VertexArray::attachVertexBuffer(VertexBuffer* buffer, GLuint attributeIndex)
	{
		glBindVertexArray(m_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, buffer->getHandle());
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, buffer->getStride() * buffer->getCount(), GL_FLOAT, GL_FALSE, buffer->getStride(), (void*)0);
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
		glBindVertexArray(m_vaoId);
		if (m_indexBuffer == nullptr)
		{
			glDrawArrays(GL_TRIANGLES, 0, m_elementCount);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, nullptr);
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
