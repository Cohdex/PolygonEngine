#pragma once

#include <GL/glew.h>

namespace plgn
{
	class MeshUtil
	{
	private:
		MeshUtil() = delete;
		MeshUtil(const MeshUtil&) = delete;
		MeshUtil& operator=(const MeshUtil&) = delete;

	public:
		static GLuint createTorus(float centerRadius, float outerRadius, int rings, int segments, int* numElements);
	};
}
