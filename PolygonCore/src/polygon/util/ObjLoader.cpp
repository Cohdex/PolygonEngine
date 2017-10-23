#include "stdafx.h"

namespace plgn
{
	VertexArray* ObjLoader::load(const char* filename)
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
				tempTexCoords.push_back(texCoord);
			}
			else if (std::strcmp(lineHeader, "f") == 0)
			{
				unsigned int vi0[3], vi1[3], vi2[3];
				std::fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vi0[0], &vi0[2], &vi0[1],
					&vi1[0], &vi1[2], &vi1[1],
					&vi2[0], &vi2[2], &vi2[1]);

				Face f;
				f.v0.pi = vi0[0] - 1;
				f.v0.ni = vi0[1] - 1;
				f.v0.ti = vi0[2] - 1;
				f.v1.pi = vi1[0] - 1;
				f.v1.ni = vi1[1] - 1;
				f.v1.ti = vi1[2] - 1;
				f.v2.pi = vi2[0] - 1;
				f.v2.ni = vi2[1] - 1;
				f.v2.ti = vi2[2] - 1;
				tempFaces.push_back(f);
			}
		}

		std::fclose(file);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<GLuint> indices;

		std::unordered_map<VertexIndex, GLuint, VertexIndexHasher> indexCache;

		for (const Face& f : tempFaces)
		{
			auto v0 = indexCache.find(f.v0);
			if (v0 != indexCache.end())
				indices.push_back(v0->second);
			else
			{
				GLuint index = positions.size();

				positions.push_back(tempPositions[f.v0.pi]);
				normals.push_back(tempNormals[f.v0.ni]);
				texCoords.push_back(tempTexCoords[f.v0.ti]);

				indices.push_back(index);
				indexCache[f.v0] = index;
			}

			auto v1 = indexCache.find(f.v1);
			if (v1 != indexCache.end())
				indices.push_back(v1->second);
			else
			{
				GLuint index = positions.size();

				positions.push_back(tempPositions[f.v1.pi]);
				normals.push_back(tempNormals[f.v1.ni]);
				texCoords.push_back(tempTexCoords[f.v1.ti]);

				indices.push_back(index);
				indexCache[f.v1] = index;
			}

			auto v2 = indexCache.find(f.v2);
			if (v2 != indexCache.end())
				indices.push_back(v2->second);
			else
			{
				GLuint index = positions.size();

				positions.push_back(tempPositions[f.v2.pi]);
				normals.push_back(tempNormals[f.v2.ni]);
				texCoords.push_back(tempTexCoords[f.v2.ti]);

				indices.push_back(index);
				indexCache[f.v2] = index;
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