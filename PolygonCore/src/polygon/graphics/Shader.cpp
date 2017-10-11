#include "stdafx.h"

namespace plgn
{
	static bool compileShader(GLuint shader, const char* source)
	{
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		return status != GL_FALSE;
	}

	static std::string getShaderLog(GLuint shader)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> logBuffer;
		logBuffer.reserve(logLength);
		glGetShaderInfoLog(shader, logLength, nullptr, logBuffer.data());
		return std::string(logBuffer.data());
	}

	static bool linkProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		return status != GL_FALSE;
	}

	static std::string getProgramLog(GLuint program)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> logBuffer;
		logBuffer.reserve(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, logBuffer.data());
		return std::string(logBuffer.data());
	}

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		if (!compileShader(vertexShader, vertexSource.c_str()))
		{
			std::string log = getShaderLog(vertexShader);
			std::cout << "Failed to compile vertex shader:" << std::endl;
			std::cout << log << std::endl;
			glDeleteShader(vertexShader);
			throw 1;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (!compileShader(fragmentShader, fragmentSource.c_str()))
		{
			std::string log = getShaderLog(fragmentShader);
			std::cout << "Failed to compile fragment shader:" << std::endl;
			std::cout << log << std::endl;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			throw 2;
		}

		m_program = glCreateProgram();
		bool success = linkProgram(m_program, vertexShader, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (!success)
		{
			std::string log = getProgramLog(m_program);
			std::cout << "Failed to link shader:" << std::endl;
			std::cout << log << std::endl;
			glDeleteProgram(m_program);
			throw 3;
		}
	}

	void Shader::destroy()
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}

	void Shader::use() const
	{
		glUseProgram(m_program);
	}
}
