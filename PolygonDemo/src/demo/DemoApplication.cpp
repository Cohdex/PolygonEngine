#include "stdafx.h"

static const char* simpleVertexShader = R"(
	#version 450 core
	#line 18

	layout(location = 0) in vec3 in_position;
	layout(location = 1) in vec3 in_color;

	out VS_OUT
	{
		vec3 color;
	} vs_out;

	uniform mat4 projectionMatrix = mat4(1.0);

	void main()
	{
		gl_Position = projectionMatrix * vec4(in_position, 1.0);
		vs_out.color = in_color;
	}
)";

static const char* simpleFragmentShader = R"(
	#version 450 core
	#line 28

	in VS_OUT
	{
		vec3 color;
	} fs_in;

	layout(location = 0) out vec4 fragColor;

	void main()
	{
		vec3 color = fs_in.color;
		fragColor = vec4(color, 1.0);
	}
)";

static GLuint createShader(const char* vertexSource, const char* fragmentSource)
{
	GLint status;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(vertexShader, logSize, nullptr, log.data());
		glDeleteShader(vertexShader);
		std::cout << "Failed to compile vertex shader:\n" << log.data() << std::endl;
		return 0;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(fragmentShader, logSize, nullptr, log.data());
		glDeleteShader(vertexShader);
		glDeleteProgram(fragmentShader);
		std::cout << "Failed to compile fragment shader:\n" << log.data() << std::endl;
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteProgram(fragmentShader);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetProgramInfoLog(program, logSize, nullptr, log.data());
		glDeleteProgram(program);
		std::cout << "Failed to compile fragment shader:\n" << log.data() << std::endl;
		return 0;
	}

	return program;
}

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 1920, 1080)
	{
	}

	void DemoApplication::init()
	{
		float pw = getWidth() / (float)glm::min(getWidth(), getHeight());
		float ph = getHeight() / (float)glm::min(getWidth(), getHeight());
		m_projectionMatrix = glm::ortho(-pw, pw, -ph, ph, 1.0f, -1.0f);

		m_simpleShader = createShader(simpleVertexShader, simpleFragmentShader);
		if (m_simpleShader == 0)
		{
			glfwTerminate();
			std::cin.get();
			throw 1;
		}
		glUseProgram(m_simpleShader);
		glUniformMatrix4fv(glGetUniformLocation(m_simpleShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

		std::vector<GLfloat> vertices;
		float r = 142 / 255.0f;
		float g = 41 / 255.0f;
		float b = 109 / 255.0f;
		r = 0.0f;
		g = -1.0f;
		b = -2.0f;
		std::srand((unsigned int)std::time(nullptr));
		constexpr int segments = 128;
		for (int i = 0; i < segments; i++)
		{
			//r = (std::rand() % 256) / 255.0f;
			//g = (std::rand() % 256) / 255.0f;
			//b = (std::rand() % 256) / 255.0f;
			GLfloat x0 = (GLfloat)std::cos((double)i / segments * M_PI * 2) * 0.8f;
			GLfloat y0 = (GLfloat)std::sin((double)i / segments * M_PI * 2) * 0.8f;
			GLfloat x1 = (GLfloat)std::cos((double)(i + 1) / segments * M_PI * 2) * 0.8f;
			GLfloat y1 = (GLfloat)std::sin((double)(i + 1) / segments * M_PI * 2) * 0.8f;

			vertices.push_back(x0);
			vertices.push_back(y0);
			vertices.push_back(r);
			vertices.push_back(g);
			vertices.push_back(b);

			vertices.push_back(x1);
			vertices.push_back(y1);
			vertices.push_back(r);
			vertices.push_back(g);
			vertices.push_back(b);

			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			//vertices.push_back(1.2f);
			//vertices.push_back(1.2f);
			//vertices.push_back(1.2f);
			vertices.push_back(1.0f);
			vertices.push_back(2.0f);
			vertices.push_back(3.0f);
		}
		m_numVertices = vertices.size();

		glGenBuffers(1, &m_vbo);
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void DemoApplication::update(double deltaTime)
	{
		if (wasKeyPressed(GLFW_KEY_ESCAPE))
		{
			stop();
		}
	}

	void DemoApplication::render()
	{
		//glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(m_simpleShader);
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glBindVertexArray(0);
		glUseProgram(0);
	}
}