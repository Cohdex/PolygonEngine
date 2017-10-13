#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

namespace plgn
{
	class Shader
	{
	private:
		GLuint m_programId;

	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void destroy();

		GLuint getProgramHandle() const { return m_programId; }

		void use() const;

		GLint getUniformLocation(const std::string& uniformName) const;

		void setUniform(const std::string& uniformName, float x);
		void setUniform(const std::string& uniformName, int x);

		void setUniform(const std::string& uniformName, const glm::vec2& v);
		void setUniform(const std::string& uniformName, const glm::vec3& v);
		void setUniform(const std::string& uniformName, const glm::vec4& v);

		void setUniform(const std::string& uniformName, const glm::mat2& m);
		void setUniform(const std::string& uniformName, const glm::mat3& m);
		void setUniform(const std::string& uniformName, const glm::mat4& m);
	};
}
