#pragma once

#include "Mesh.h"

namespace plgn
{
	class Model
	{
	private:
		std::vector<std::unique_ptr<Mesh>> m_meshes;

	public:
		Model(const std::string& filename);
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void draw() const;
	};
}
