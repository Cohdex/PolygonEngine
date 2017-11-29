#pragma once

#include <GL/glew.h>

#include "../graphics/vertexbuffer/VertexArray.h"

namespace plgn
{
	class ObjLoader
	{
	public:
		ObjLoader() = delete;
		ObjLoader(const ObjLoader&) = delete;
		ObjLoader& operator=(const ObjLoader&) = delete;

		static VertexArray* load(const char* filename);
	};
}
