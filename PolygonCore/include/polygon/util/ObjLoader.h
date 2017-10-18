#pragma once

#include <GL/glew.h>

namespace plgn
{
	class ObjLoader
	{
	public:
		ObjLoader() = delete;
		ObjLoader(const ObjLoader&) = delete;
		ObjLoader& operator=(const ObjLoader&) = delete;

		static GLuint load(const char* filename, int* numElements);
	};
}
