#include "stdafx.h"

namespace plgn
{
	GLuint ObjLoader::load(const char* filename, int* numElements)
	{
		std::FILE* file = std::fopen(filename, "r");
		if (file == nullptr)
		{
			std::cout << "Couldn't open file: " << filename << std::endl;
			throw 1;
		}

		std::vector<glm::vec3> tempPositions;
		std::vector<glm::vec3> tempNormals;
		std::vector<glm::vec2> tempTexCoords;

		struct VertexIndex
		{
			unsigned int pi, ni, ti;

			bool operator==(const VertexIndex& other) const
			{
				return pi == other.pi && ni == other.ni && ti == other.ti;
			}
		};

		struct VertexIndexHasher
		{
			size_t operator()(const VertexIndex& vi) const
			{
				return (vi.pi * 31 + vi.ni) * 31 + vi.ti;
			}
		};

		struct Face
		{
			VertexIndex v0, v1, v2;
		};

		std::vector<Face> tempFaces;

		while (true)
		{
			char lineHeader[128];
			int res = std::fscanf(file, "%s", lineHeader);
			if (res == EOF)
			{
				break;
			}
			if (std::strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 position;
				std::fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
				tempPositions.push_back(position);
			}
			else if (std::strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				std::fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			else if (std::strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 texCoord;
				std::fscanf(file, "%f %f\n", &texCoord.s, &texCoord.t);
				tempTexCoords.push_back(texCoord / 65.7549f);
			}
			else if (std::strcmp(lineHeader, "f") == 0)
			{
				unsigned int vi0[3], vi1[3], vi2[3], vi3[3];
				std::fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vi0[0], &vi0[1], &vi0[2],
					&vi1[0], &vi1[1], &vi1[2],
					&vi2[0], &vi2[1], &vi2[2],
					&vi3[0], &vi3[1], &vi3[2]);

				Face f0;
				f0.v0.pi = vi0[0] - 1;
				f0.v0.ni = vi0[1] - 1;
				f0.v0.ti = vi0[2] - 1;
				f0.v1.pi = vi1[0] - 1;
				f0.v1.ni = vi1[1] - 1;
				f0.v1.ti = vi1[2] - 1;
				f0.v2.pi = vi2[0] - 1;
				f0.v2.ni = vi2[1] - 1;
				f0.v2.ti = vi2[2] - 1;
				tempFaces.push_back(f0);

				Face f1;
				f1.v0.pi = vi2[0] - 1;
				f1.v0.ni = vi2[1] - 1;
				f1.v0.ti = vi2[2] - 1;
				f1.v1.pi = vi3[0] - 1;
				f1.v1.ni = vi3[1] - 1;
				f1.v1.ti = vi3[2] - 1;
				f1.v2.pi = vi0[0] - 1;
				f1.v2.ni = vi0[1] - 1;
				f1.v2.ti = vi0[2] - 1;
				tempFaces.push_back(f1);
			}
		}

		std::fclose(file);

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoord;
		};

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		std::unordered_map<VertexIndex, GLuint, VertexIndexHasher> indexCache;

		for (const Face& f : tempFaces)
		{
			auto v0 = indexCache.find(f.v0);
			if (v0 != indexCache.end())
				indices.push_back(v0->second);
			else
			{
				GLuint index = vertices.size();
				Vertex vert;
				vert.position = tempPositions[f.v0.pi];
				vert.normal = tempNormals[f.v0.ni];
				vert.texCoord = tempTexCoords[f.v0.ti];
				vertices.push_back(vert);
				indices.push_back(index);
				indexCache[f.v0] = index;
			}

			auto v1 = indexCache.find(f.v1);
			if (v1 != indexCache.end())
				indices.push_back(v1->second);
			else
			{
				GLuint index = vertices.size();
				Vertex vert;
				vert.position = tempPositions[f.v1.pi];
				vert.normal = tempNormals[f.v1.ni];
				vert.texCoord = tempTexCoords[f.v1.ti];
				vertices.push_back(vert);
				indices.push_back(index);
				indexCache[f.v1] = index;
			}

			auto v2 = indexCache.find(f.v2);
			if (v2 != indexCache.end())
				indices.push_back(v2->second);
			else
			{
				GLuint index = vertices.size();
				Vertex vert;
				vert.position = tempPositions[f.v2.pi];
				vert.normal = tempNormals[f.v2.ni];
				vert.texCoord = tempTexCoords[f.v2.ti];
				vertices.push_back(vert);
				indices.push_back(index);
				indexCache[f.v2] = index;
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