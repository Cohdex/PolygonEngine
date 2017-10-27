#include "stdafx.h"

namespace plgn
{
	VertexArray* MeshUtil::createPlane(float width, float height, float u, float v)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		std::vector<GLuint> indices;

		positions.emplace_back(-width / 2, -height / 2, 0);
		positions.emplace_back(width / 2, -height / 2, 0);
		positions.emplace_back(width / 2, height / 2, 0);
		positions.emplace_back(-width / 2, height / 2, 0);

		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);

		texCoords.emplace_back(0, 0);
		texCoords.emplace_back(u, 0);
		texCoords.emplace_back(u, v);
		texCoords.emplace_back(0, v);

		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(3);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(3);

		VertexArray* vao = new VertexArray(indices.size());
		vao->setIndexBuffer(new IndexBuffer(indices));
		vao->addVertexBuffer(new VertexBuffer(positions), 0);
		vao->addVertexBuffer(new VertexBuffer(normals), 1);
		vao->addVertexBuffer(new VertexBuffer(texCoords), 2);
		vao->addVertexBuffer(new VertexBuffer(tangents), 3);

		return vao;
	}

	VertexArray* MeshUtil::createCube(float width, float height, float depth)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		std::vector<GLuint> indices;

		float w = width / 2;
		float h = height / 2;
		float d = depth / 2;

		positions.emplace_back(w, -h,  d);
		positions.emplace_back(w, -h, -d);
		positions.emplace_back(w,  h, -d);
		positions.emplace_back(w,  h,  d);
		normals.emplace_back(1, 0, 0);
		normals.emplace_back(1, 0, 0);
		normals.emplace_back(1, 0, 0);
		normals.emplace_back(1, 0, 0);
		tangents.emplace_back(0, 0, -1);
		tangents.emplace_back(0, 0, -1);
		tangents.emplace_back(0, 0, -1);
		tangents.emplace_back(0, 0, -1);

		positions.emplace_back(-w, -h, -d);
		positions.emplace_back(-w, -h,  d);
		positions.emplace_back(-w,  h,  d);
		positions.emplace_back(-w,  h, -d);
		normals.emplace_back(-1, 0, 0);
		normals.emplace_back(-1, 0, 0);
		normals.emplace_back(-1, 0, 0);
		normals.emplace_back(-1, 0, 0);
		tangents.emplace_back(0, 0, 1);
		tangents.emplace_back(0, 0, 1);
		tangents.emplace_back(0, 0, 1);
		tangents.emplace_back(0, 0, 1);

		positions.emplace_back(-w, h,  d);
		positions.emplace_back( w, h,  d);
		positions.emplace_back( w, h, -d);
		positions.emplace_back(-w, h, -d);
		normals.emplace_back(0, 1, 0);
		normals.emplace_back(0, 1, 0);
		normals.emplace_back(0, 1, 0);
		normals.emplace_back(0, 1, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);

		positions.emplace_back(-w, -h, -d);
		positions.emplace_back( w, -h, -d);
		positions.emplace_back( w, -h,  d);
		positions.emplace_back(-w, -h,  d);
		normals.emplace_back(0, -1, 0);
		normals.emplace_back(0, -1, 0);
		normals.emplace_back(0, -1, 0);
		normals.emplace_back(0, -1, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);

		positions.emplace_back(-w, -h, d);
		positions.emplace_back( w, -h, d);
		positions.emplace_back( w,  h, d);
		positions.emplace_back(-w,  h, d);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, 0, 1);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);
		tangents.emplace_back(1, 0, 0);

		positions.emplace_back( w, -h, -d);
		positions.emplace_back(-w, -h, -d);
		positions.emplace_back(-w,  h, -d);
		positions.emplace_back( w,  h, -d);
		normals.emplace_back(0, 0, -1);
		normals.emplace_back(0, 0, -1);
		normals.emplace_back(0, 0, -1);
		normals.emplace_back(0, 0, -1);
		tangents.emplace_back(-1, 0, 0);
		tangents.emplace_back(-1, 0, 0);
		tangents.emplace_back(-1, 0, 0);
		tangents.emplace_back(-1, 0, 0);

		for (int i = 0; i < 6; i++)
		{
			texCoords.emplace_back(0, 0);
			texCoords.emplace_back(1, 0);
			texCoords.emplace_back(1, 1);
			texCoords.emplace_back(0, 1);

			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 3);
			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 2);
		}

		VertexArray* vao = new VertexArray(indices.size());
		vao->setIndexBuffer(new IndexBuffer(indices));
		vao->addVertexBuffer(new VertexBuffer(positions), 0);
		vao->addVertexBuffer(new VertexBuffer(normals), 1);
		vao->addVertexBuffer(new VertexBuffer(texCoords), 2);
		vao->addVertexBuffer(new VertexBuffer(tangents), 3);

		return vao;
	}

	VertexArray* MeshUtil::createTorus(float centerRadius, float outerRadius, int rings, int segments)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		std::vector<GLuint> indices;

		for (int r = 0; r <= rings; r++)
		{
			float u = (float)r / rings * glm::two_pi<float>();

			glm::vec3 w = glm::vec3(glm::cos(u), 0.0f, glm::sin(u));
			glm::vec3 q = w * centerRadius;

			glm::vec3 tangent(w.z, 0.0f, -w.x);

			for (int s = 0; s <= segments; s++)
			{
				float v = (float)s / segments * glm::two_pi<float>();

				glm::vec3 n = glm::cos(v) * w + glm::vec3(0.0f, glm::sin(v), 0.0f);
				glm::vec3 p = q + outerRadius * n;
				glm::vec2 t = glm::vec2((1.0f - u / glm::two_pi<float>()) * 2.0f, v / glm::two_pi<float>());

				positions.push_back(p);
				normals.push_back(n);
				texCoords.push_back(t);
				tangents.push_back(tangent);
			}
		}

		auto getIndex = [segments](int r, int s) {
			return r * (segments + 1) + s;
		};

		for (int r = 0; r < rings; r++)
		{
			for (int s = 0; s < segments; s++)
			{
				indices.push_back(getIndex(r, s));
				indices.push_back(getIndex(r, s + 1));
				indices.push_back(getIndex(r + 1, s));

				indices.push_back(getIndex(r + 1, s));
				indices.push_back(getIndex(r, s + 1));
				indices.push_back(getIndex(r + 1, s + 1));
			}
		}

		VertexArray* vao = new VertexArray(indices.size());
		vao->setIndexBuffer(new IndexBuffer(indices));
		vao->addVertexBuffer(new VertexBuffer(positions), 0);
		vao->addVertexBuffer(new VertexBuffer(normals), 1);
		vao->addVertexBuffer(new VertexBuffer(texCoords), 2);
		vao->addVertexBuffer(new VertexBuffer(tangents), 3);

		return vao;
	}
}
