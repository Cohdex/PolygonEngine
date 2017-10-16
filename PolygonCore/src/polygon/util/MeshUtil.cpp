#include "stdafx.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	Vertex()
	{
	}
	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tc)
		: position(pos), normal(norm), texCoord(tc)
	{
	}
};

namespace plgn
{
	GLuint MeshUtil::createTorus(float centerRadius, float outerRadius, int rings, int segments, int* numElements)
	{
		std::vector<Vertex> vertices;
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

				vertices.emplace_back();
				Vertex& vert = vertices.back();

				vert.position = q;
				vert.normal = n;
				vert.texCoord = glm::vec2(1.0f - u / glm::two_pi<float>(), v / glm::two_pi<float>());
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

		*numElements = indices.size();

		GLuint vbo, ebo, vao;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return vao;
	}
}
