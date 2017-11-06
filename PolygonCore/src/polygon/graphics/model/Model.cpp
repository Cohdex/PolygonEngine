#include "stdafx.h"

namespace plgn
{
	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_GenUVCoords |
			aiProcess_FlipUVs);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			std::cout << "Error loading model: " << importer.GetErrorString() << std::endl;
			throw 1;
		}

		processNode(scene, scene->mRootNode);
	}

	void Model::processNode(const aiScene* scene, aiNode* node)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(scene, mesh);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(scene, node->mChildren[i]);
		}
	}

	void Model::processMesh(const aiScene* scene, aiMesh* mesh)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			positions.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			texCoords.emplace_back(mesh->mTextureCoords[i]->x, mesh->mTextureCoords[i]->y);
			tangents.emplace_back(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		}

		std::vector<GLuint> indices;

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		VertexArray* vao = new VertexArray(indices.size());
		vao->setIndexBuffer(new IndexBuffer(indices));
		vao->addVertexBuffer(new VertexBuffer(positions), 0);
		vao->addVertexBuffer(new VertexBuffer(normals), 1);
		vao->addVertexBuffer(new VertexBuffer(texCoords), 2);
		vao->addVertexBuffer(new VertexBuffer(tangents), 3);

		m_meshes.push_back(std::move(std::make_unique<Mesh>(vao)));
	}

	void Model::draw() const
	{
		for (const std::unique_ptr<Mesh>& m : m_meshes)
		{
			m->draw();
		}
	}
}
