#pragma once

#include "Mesh.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace plgn
{
	class Model
	{
	private:
		std::vector<std::unique_ptr<Mesh>> m_meshes;

		void processNode(const aiScene* scene, aiNode* node);
		void processMesh(const aiScene* scene, aiMesh* mesh);

	public:
		Model(const std::string& filename);
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void draw() const;
	};
}
