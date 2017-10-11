#include "stdafx.h"

static const std::string simpleVertexShader = R"(
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

static const std::string simpleFragmentShader = R"(
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

namespace demo
{
	DemoApplication::DemoApplication() : Application("Demo Application", 640, 480)
	{
	}

	void DemoApplication::init()
	{
		float pw = getWidth() / (float)glm::min(getWidth(), getHeight());
		float ph = getHeight() / (float)glm::min(getWidth(), getHeight());
		m_projectionMatrix = glm::ortho(-pw, pw, -ph, ph, 1.0f, -1.0f);

		m_simpleShader = std::make_unique<plgn::Shader>(simpleVertexShader, simpleFragmentShader);
		m_simpleShader->use();
		m_simpleShader->setUniform("projectionMatrix", m_projectionMatrix);
		
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color;

			Vertex(const glm::vec2& pos, const glm::vec3& col) : position(pos), color(col) { }
		};
		std::vector<Vertex> vertices;
		glm::vec3 color;
		color.r = 142 / 255.0f;
		color.g = 41 / 255.0f;
		color.b = 109 / 255.0f;
		//color.r = 0.0f;
		//color.g = -1.0f;
		//color.b = -2.0f;
		std::srand((unsigned int)std::time(nullptr));
		constexpr int segments = 128;
		for (int i = 0; i < segments; i++)
		{
			//r = (std::rand() % 256) / 255.0f;
			//g = (std::rand() % 256) / 255.0f;
			//b = (std::rand() % 256) / 255.0f;
			glm::vec2 p0, p1;
			p0.x = (GLfloat)std::cos((double)i / segments * M_PI * 2) * 0.8f;
			p0.y = (GLfloat)std::sin((double)i / segments * M_PI * 2) * 0.8f;
			p1.x = (GLfloat)std::cos((double)(i + 1) / segments * M_PI * 2) * 0.8f;
			p1.y = (GLfloat)std::sin((double)(i + 1) / segments * M_PI * 2) * 0.8f;

			vertices.emplace_back(p0, color);
			vertices.emplace_back(p1, color);
			vertices.emplace_back(glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 2.0f, 3.0f));
		}
		m_numVertices = vertices.size();

		glGenBuffers(1, &m_vbo);
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
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

		m_simpleShader->use();
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void DemoApplication::dispose()
	{
		m_simpleShader->destroy();
		m_simpleShader.reset();
	}
}
