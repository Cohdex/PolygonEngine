#pragma once

namespace plgn
{
	class Mesh
	{
	private:
		std::unique_ptr<const VertexArray> m_vao;

	public:
		Mesh(const VertexArray* vao);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void draw() const;
	};
}
