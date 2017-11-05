#include "stdafx.h"

namespace plgn
{
	Mesh::Mesh(const VertexArray* vao)
		: m_vao(vao)
	{
	}

	void Mesh::draw() const
	{
		m_vao->draw();
	}
}
