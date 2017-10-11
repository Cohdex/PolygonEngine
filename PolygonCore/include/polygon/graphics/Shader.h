#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>

namespace plgn
{
	class Shader
	{
	private:
		GLuint m_program;

	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);

		void destroy();

		GLuint getProgramHandle() const { return m_program; }

		void use() const;
	};
}
