#pragma once

#include <GL/glew.h>

#include <vector>
#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace plgn
{
	class VertexArray
	{
	private:
		GLuint m_vaoId;

		GLsizei m_count;

		std::vector<VertexBuffer*> m_vertexBuffers;
		std::unique_ptr<IndexBuffer> m_indexBuffer;

	public:
		VertexArray(GLsizei count);

		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		inline GLuint getHandle() const { return m_vaoId; };
		inline GLsizei getCount() const { return m_count; }

		void addVertexBuffer(VertexBuffer* buffer, GLuint attributeIndex);
		void setIndexBuffer(IndexBuffer* buffer);

		void draw() const;

		void destroy();
	};
}
