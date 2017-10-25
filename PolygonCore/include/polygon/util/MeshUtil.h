#pragma once

#include <GL/glew.h>

#include "../graphics/buffer/VertexArray.h"

namespace plgn
{
	class MeshUtil
	{
	private:
		MeshUtil() = delete;
		MeshUtil(const MeshUtil&) = delete;
		MeshUtil& operator=(const MeshUtil&) = delete;

	public:
		static VertexArray* createPlane(float width, float height);
		static VertexArray* createTorus(float centerRadius, float outerRadius, int rings, int segments);
	};
}
