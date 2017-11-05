#include "stdafx.h"

namespace plgn
{
	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
	}

	void Model::draw() const
	{
		for (const std::unique_ptr<Mesh>& m : m_meshes)
		{
			m->draw();
		}
	}
}
