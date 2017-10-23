#include "stdafx.h"

namespace plgn
{
	VertexArray* MeshUtil::createTorus(float centerRadius, float outerRadius, int rings, int segments)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<GLuint> indices;

		for (int r = 0; r <= rings; r++)
		{
			float u = (float)r / rings * glm::two_pi<float>();

			glm::vec3 w = glm::vec3(glm::cos(u), 0.0f, glm::sin(u));
			glm::vec3 p = w * centerRadius;

			for (int s = 0; s <= segments; s++)
			{
				float v = (float)s / segments * glm::two_pi<float>();

				glm::vec3 n = glm::cos(v) * w + glm::vec3(0.0f, glm::sin(v), 0.0f);
				glm::vec3 q = p + outerRadius * n;
				glm::vec2 t = glm::vec2(1.0f - u / glm::two_pi<float>(), v / glm::two_pi<float>());

				positions.push_back(q);
				normals.push_back(n);
				texCoords.push_back(t);
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

		return vao;
	}
}
